#include "../include/Handler.h"

Handler::Handler(SessionManager& sm) : userManager_(Config::getDB().getConnectionStr()), messageManager_(Config::getDB().getConnectionStr()), dialogManager_(Config::getDB().getConnectionStr()), sessionManager_(sm), dispatcher_(sessionManager_) {
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

std::string Handler::generateToken() {
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
        dispatcher_.sendTo(client, protocol::loginResponse(false, -1, "", error));
        return;
    }

    auto [success, user_id, error] = userManager_.loginUser(j["username"], j["password"]);
    if (!success) {
        dispatcher_.sendTo(client, protocol::loginResponse(success, user_id, j["username"], "", error));
        return;
    }

    authSuccess(client, user_id, j["username"]);
}

void Handler::registerRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if(std::string error; !Validator::valid_string_field(j, "username", Validator::username, error) || !Validator::valid_string_field(j, "password", Validator::password, error)) {
        dispatcher_.sendTo(client, protocol::registerResponse(false, -1, "", error));
        return;
    }
    
    auto [success, user_id, error] = userManager_.registerUser(j["username"],j["password"]);
    if (!success) {
        dispatcher_.sendTo(client, protocol::loginResponse(success, user_id, j["username"], "", error));
        return;
    }

    authSuccess(client, user_id, j["username"]);
}

void Handler::authSuccess(const std::shared_ptr<ClientSession>& client, int id, const std::string& username) {
    client->setUser(id, username);
    client->set_is_authenticated(true);
    const std::string token = generateToken();
    userManager_.createSession(id, token);
    dispatcher_.sendTo(client, protocol::loginResponse(true, id, username, token, ""));
    sessionManager_.add(client);
}

void Handler::sendMessage(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if (!client->get_is_authenticated()) {
        std::cerr << "SORRY, not auth\n";
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

    const std::string preview = msg.getPreview();
    dialogManager_.upsertDialog(msg.sender_id, msg.dialog_id, msg.id, preview, msg.created_at_ms);
    dialogManager_.upsertDialog(msg.dialog_id, msg.sender_id, msg.id, preview, msg.created_at_ms);

    const auto receiver = sessionManager_.getSessionByDialogId(msg.dialog_id);
    if (receiver && receiver->getUserId() != msg.sender_id) {
        if (!receiver->send(protocol::sendMessage(msg))) {
            std::cerr << "Failed to send message to dialog " << msg.dialog_id << '\n';
        }
    }

    dispatcher_.sendTo(client, protocol::sendMessage(msg));
}

void Handler::setDisconnectHandler(const std::function<void(std::shared_ptr<ClientSession>)> &cb) {
    dispatcher_.onDisconnect = cb;
}

void Handler::searchUserRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if(std::string error; !Validator::valid_string_field(j, "username", Validator::search, error)) {
        dispatcher_.sendTo(client, protocol::errorMessage(error));
        return;
    }

    const auto users = userManager_.searchUsers(j["username"]);

    dispatcher_.sendTo(client, protocol::searchUserResponse(users));
}

void Handler::historyRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if(!client->get_is_authenticated()) return;
    const auto history = messageManager_.getHistory(client->getUserId(), j["peer_id"], j["last_msg_id"], j["limit"]);
    dispatcher_.sendTo(client, protocol::historyResponse(!history.empty(), j["peer_id"], history));
}

void Handler::getDialogsRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if(!client->get_is_authenticated()) return;

    const auto dialogs = dialogManager_.getUserDialogs(client->getUserId());
    std::vector<MetaDialog> metas;
    for (const auto&[peer_id, username, last_msg_text, last_msg_timestamp, last_activity_time] : dialogs)
        metas.push_back({.peer_id = peer_id, .username = username, .last_msg_text = last_msg_text, .last_msg_timestamp = last_msg_timestamp, .last_activity_time = last_activity_time});

    dispatcher_.sendTo(client, protocol::getDialogsResponse(!metas.empty(), metas));
}

void Handler::ping(const std::shared_ptr<ClientSession> &client, const nlohmann::json& j) {
    SessionManager::updateActivity(client);
}

void Handler::resumeConnectionRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    const std::string token = j.value("token", "");
    if (token.empty()) {
        std::cerr << "token empty\n";
        dispatcher_.sendTo(client, protocol::resumeConnectionResponse(false));
        return;
    }

    const auto user_id = userManager_.getUserIdByToken(token);
    if(!user_id.has_value()) {
        dispatcher_.sendTo(client, protocol::resumeConnectionResponse(false));
        return;
    }

    const int id = user_id.value();
    client->setUser(id, "");
    client->set_is_authenticated(true);
    sessionManager_.add(client);
    dispatcher_.sendTo(client, protocol::resumeConnectionResponse(true));
}