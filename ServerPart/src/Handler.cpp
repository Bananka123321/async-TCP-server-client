#include "../include/Handler.h"

Handler::Handler(SessionManager& sm) : userManager_(Config::getDB().getConnectionStr()), messageManager_(Config::getDB().getConnectionStr()),
    dialogManager_(Config::getDB().getConnectionStr()), devicesSessionManager_(Config::getDB().getConnectionStr()), temporaryTokenManager_(Config::getDB().getConnectionStr()),
    sessionManager_(sm), dispatcher_(sessionManager_) {

    handlers_["loginRequest"] = [this](const auto& client, const auto& j) {
        loginRequest(client, j);
    };

    handlers_["registerRequest"] = [this](const auto& client, const auto& j) {
        registerRequest(client, j);
    };

    handlers_["sendMessage"] = [this](const auto& client, const auto& j) {
        sendMessage(client, j);
    };

    handlers_["searchUserRequest"] = [this](const auto& client, const auto& j) {
        searchUserRequest(client, j);
    };

    handlers_["historyRequest"] = [this](const auto& client, const auto& j) {
        historyRequest(client, j);
    };

    handlers_["getDialogsRequest"] = [this](const auto& client, const auto& j) {
        getDialogsRequest(client, j);
    };

    handlers_["ping"] = [](const auto& client, const auto& j) {
        ping(client, j);
    };

    handlers_["resumeConnectionRequest"] = [this](const auto& client, const auto& j) {
        resumeConnectionRequest(client, j);
    };

    handlers_["resumeSessionRequest"] = [this](const auto& client, const auto& j) {
        resumeSessionRequest(client, j);
    };
}

void Handler::handleMessage(const std::shared_ptr<ClientSession>& client, std::string_view msg) {
    try {
        auto j = nlohmann::json::parse(msg);
        SessionManager::updateActivity(client);
        if (const std::string type = j["type"]; handlers_.contains(type)) {
            handlers_[type](client, j);
        }
        else
            std::cerr << "Unknown message type " << type << std::endl;    

    } catch (const std::exception& e) {
        std::cerr << "JSON parse ERROR " << e.what() << std::endl;
    }
}

static std::mt19937& getGlobalRNG() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

std::string Handler::generateConnectToken() {
    static std::mutex rng_mutex;
    std::lock_guard<std::mutex> lock(rng_mutex);

    auto& gen = getGlobalRNG();
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);
    std::stringstream ss;
    for(int i = 0; i < 4; i++)
        ss << std::hex << std::setw(8) << std::setfill('0') << dis(gen);
    return ss.str();
}



//============================================================================================================================



void Handler::loginRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if(std::string error; !Validator::valid_string_field(j, "username", Validator::username, error) || !Validator::valid_string_field(j, "password", Validator::password, error)) {
        dispatcher_.sendTo(client, protocol::loginResponse(false, -1, "", "", "", error));
        return;
    }

    auto [success, user_id, error] = userManager_.loginUser(j["username"], j["password"]);
    if (!success) {
        dispatcher_.sendTo(client, protocol::loginResponse(success, user_id, j["username"], "", error));
        return;
    }

    const auto sessionToken = devicesSessionManager_.createToken(user_id, "", "");
    const auto connectToken = authSuccess(client, user_id, j["username"]);

    if (!sessionToken.has_value()) {
        dispatcher_.sendTo(client, protocol::loginResponse(false, user_id, "", "", "", error));
        return;
    }
    dispatcher_.sendTo(client, protocol::loginResponse(true, user_id, j["username"], connectToken, sessionToken.value(), error));
}

void Handler::registerRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if(std::string error; !Validator::valid_string_field(j, "username", Validator::username, error) || !Validator::valid_string_field(j, "password", Validator::password, error)) {
        dispatcher_.sendTo(client, protocol::registerResponse(false, -1, "", "", "", error));
        return;
    }
    
    auto [success, user_id, error] = userManager_.registerUser(j["username"],j["password"]);
    if (!success) {
        dispatcher_.sendTo(client, protocol::registerResponse(success, user_id, j["username"], "", error));
        return;
    }

    const auto sessionToken = devicesSessionManager_.createToken(user_id, "", "");
    const auto connectToken = authSuccess(client, user_id, j["username"]);

    if (!sessionToken.has_value()) {
        dispatcher_.sendTo(client, protocol::registerResponse(false, user_id, "", "", "", error));
        return;
    }

    dispatcher_.sendTo(client, protocol::registerResponse(true, user_id, j["username"], connectToken, sessionToken.value(), error));
}

std::string Handler::authSuccess(const std::shared_ptr<ClientSession>& client, const int id, const std::string& username) {
    client->setUser(id, username);
    client->setIsAuthenticated(true);
    sessionManager_.add(client);

    const std::string connectToken = generateConnectToken();
    client->setTempToken(connectToken);
    temporaryTokenManager_.createSession(id, connectToken);

    return connectToken;
}

void Handler::sendMessage(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if (!client->getIsAuthenticated()) {
        dispatcher_.sendTo(client, protocol::errorMessage("Not authenticated"));
        return;
    }
    
    Message msg;
    try {
        msg = j.at("data").get<Message>();
    } catch ([[maybe_unused]] const std::exception& e) {
        dispatcher_.sendTo(client, protocol::errorMessage("Invalid message format"));
        return;
    }

    if (msg.sender_id != client->getUserId()) {
        dispatcher_.sendTo(client, protocol::errorMessage("Forbidden: sender_id mismatch"));
        return;
    }

    msg.sender_id = client->getUserId();

    if(const auto error = Validator::validateMessage(msg); error.has_value()) {
        dispatcher_.sendTo(client, protocol::errorMessage(error.value()));
        return;
    }

    if (std::holds_alternative<TextContent>(msg.payload)) {
        auto&[text] = std::get<TextContent>(msg.payload);
        std::string cleanText = Validator::sanitize(text);
        if (cleanText.empty()) {
            dispatcher_.sendTo(client, protocol::errorMessage("Message is empty"));
            return;
        }
        text = std::move(cleanText);
    }

    msg.created_at_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    const auto saved_id = messageManager_.saveMessage(msg);
    if (!saved_id.has_value()) {
        dispatcher_.sendTo(client, protocol::errorMessage("Database error"));
        return;
    }

    msg.id = saved_id.value();
    dialogManager_.updateLastMessage(msg);

    const auto& participants = dialogManager_.getDialogParticipants(msg.dialog_id);
    for (const auto participant : participants) {
        if (participant == msg.sender_id) {
            continue;
        }

        if (const auto& receiver = sessionManager_.getByUserId(participant)) {
            if (!receiver->send(protocol::sendMessage(msg))) {
                std::cerr << "Failed to send message to dialog " << msg.dialog_id << '\n';
            }
        }
    }

    //Последующе подтверждение отправки сообщения(статус: отправлено)
}

void Handler::setDisconnectHandler(const std::function<void(std::shared_ptr<ClientSession>)> &cb) {
    dispatcher_.onDisconnect = cb;
}

void Handler::searchUserRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if (!client->getIsAuthenticated()) {
        dispatcher_.sendTo(client, protocol::errorMessage("Not authenticated"));
        return;
    }

    if(std::string error; !Validator::valid_string_field(j, "username", Validator::search, error)) {
        dispatcher_.sendTo(client, protocol::errorMessage(error));
        return;
    }

    const auto users = userManager_.searchUsers(j["username"].get<std::string>());
    dispatcher_.sendTo(client, protocol::searchUserResponse(users));
}

void Handler::historyRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if (!client->getIsAuthenticated()) {
        dispatcher_.sendTo(client, protocol::errorMessage("Not authenticated"));
        return;
    }

    if (!j.contains("dialog_id") || !j.contains("last_msg_id") || !j.contains("limit")) {
        dispatcher_.sendTo(client, protocol::errorMessage("Missing required fields"));
        return;
    }

    int64_t dialog_id;
    int64_t last_msg_id;
    int limit;

    try {
        dialog_id = j["dialog_id"].get<int64_t>();
        last_msg_id = j["last_msg_id"].get<int64_t>();
        limit = j["limit"].get<int>();
    } catch (const std::exception& e) {
        dispatcher_.sendTo(client, protocol::errorMessage("Invalid parameter types. " + std::string(e.what())));
        return;
    }

    if (limit <= 0 || limit > 100) {
        dispatcher_.sendTo(client, protocol::errorMessage("Limit must be between 1 and 100"));
        return;
    }

    auto participants = dialogManager_.getDialogParticipants(dialog_id);
    if (std::ranges::find(participants, client->getUserId()) == participants.end()) {
        dispatcher_.sendTo(client, protocol::errorMessage("You are not a participant of this dialog"));
        return;
    }

    const auto history = messageManager_.getHistory(dialog_id, last_msg_id, limit);

    dispatcher_.sendTo(client, protocol::historyResponse(!history.empty(), dialog_id, history));
}

void Handler::getDialogsRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if(!client->getIsAuthenticated()) {
        dispatcher_.sendTo(client, protocol::errorMessage("Not authenticated"));
        return;
    }

    auto dialogs = dialogManager_.getUserDialogs(client->getUserId());
    dispatcher_.sendTo(client, protocol::getDialogsResponse(!dialogs.empty(), std::move(dialogs)));
}

void Handler::ping(const std::shared_ptr<ClientSession> &client, const nlohmann::json& j) {
    SessionManager::updateActivity(client);
    std::cerr << "ping\n";
}

void Handler::resumeConnectionRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    const std::string token = j.value("token", "");

    if (token.empty()) {
        std::cerr << "token empty\n";
        dispatcher_.sendTo(client, protocol::resumeConnectionResponse(false));
        return;
    }

    if (!temporaryTokenManager_.isValid(token)) {
        std::cerr << "Invalid token\n";
        dispatcher_.sendTo(client, protocol::resumeConnectionResponse(false));
        return;
    }

    const auto user_id = temporaryTokenManager_.getUserIdByToken(token);
    if(!user_id.has_value()) {
        std::cerr << "user_id empty\n";
        dispatcher_.sendTo(client, protocol::resumeConnectionResponse(false));
        return;
    }

    const auto username = userManager_.getUsername(user_id.value());
    if (!username.has_value()) {
        std::cerr << "username empty\n";
        dispatcher_.sendTo(client, protocol::resumeConnectionResponse(false));
        return;
    }

    client->setUser(user_id.value(), username.value());
    client->setIsAuthenticated(true);
    sessionManager_.add(client);
    dispatcher_.sendTo(client, protocol::resumeConnectionResponse(true));
}

void Handler::resumeSessionRequest(const std::shared_ptr<ClientSession> &client, const nlohmann::json &j) {
    const std::string token = j.value("token", "");

    if (token.empty()) {
        std::cerr << "token empty\n";
        dispatcher_.sendTo(client, protocol::resumeSessionResponse(false));
        return;
    }

    if (!devicesSessionManager_.isValid(token)) {
        std::cerr << "token is not valid\n";
        dispatcher_.sendTo(client, protocol::resumeSessionResponse(false));
        return;
    }

    const auto user_id = devicesSessionManager_.getUserIdByToken(token);
    if (!user_id.has_value()) {
        std::cerr << "user_id empty\n";
        dispatcher_.sendTo(client, protocol::resumeSessionResponse(false));
        return;
    }

    const auto username = userManager_.getUsername(user_id.value());
    if (!username.has_value()) {
        std::cerr << "username empty\n";
        dispatcher_.sendTo(client, protocol::resumeSessionResponse(false));
        return;
    }

    devicesSessionManager_.updateActivity(token);
    const auto tempToken = authSuccess(client, user_id.value(), username.value());
    dispatcher_.sendTo(client, protocol::resumeSessionResponse(true, tempToken));
}
