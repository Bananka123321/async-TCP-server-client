#include "Handler.h"

Handler::Handler() {
    handlers_["loginResponse"] = [this] (const nlohmann::json& j) {
        onLoginResponse(j["success"], j["user_id"], j["username"], j["token"], j["error"]);
    };

    handlers_["registerResponse"] = [this] (const nlohmann::json& j) {
        onRegisterResponse(j["success"], j["user_id"], j["username"], j["token"], j["error"]);
    };

    handlers_["sendMessage"] = [this] (const nlohmann::json& j) {
        onSendMessage(j["data"]);
    };

    handlers_["searchUserResponse"] = [this] (const nlohmann::json& j) {
        onSearchUserResponse(j["result"]);
    };

    handlers_["error"] = [this] (const nlohmann::json& j) {
        onErrorMessage(j["message"]);
    };

    handlers_["historyResponse"] = [this] (const nlohmann::json& j) {
        onHistoryResponse(j["success"], j["peer_id"], j["messages"], j["error"]);
    };

    handlers_["getDialogsResponse"] = [this] (const nlohmann::json& j) {
        onGetDialogsResponse(j["success"], j["dialogs"], j["error"]);
    };

    handlers_["resumeConnectionResponse"] = [this] (const nlohmann::json& j) {
        onResumeConnectionResponse(j["success"]);
    };
}

void Handler::handleMessage(std::string_view msg) {
    try {
        auto j = nlohmann::json::parse(msg);

        if (const std::string type = j["type"]; handlers_.contains(type))
            handlers_[type](j);
        else
            std::cerr << "Unknown message type: " << type << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse ERROR: " << e.what() << std::endl;
    }
}

//=============================================================================================

void Handler::onLoginResponse(const bool success, const int user_id, const std::string& login, const std::string& token, const std::string& reason) {
    if (success) {
        emit S_loginSuccess(login, user_id, token);
    } else
        emit S_loginFailed(reason);
}

void Handler::onRegisterResponse(const bool success, const int user_id, const std::string& login, const std::string& token, const std::string& reason) {
    if (success) {
        emit S_registerSuccess(login, user_id, token);
    } else
        emit S_registerFailed(reason);
}

void Handler::onSendMessage(const Message& msg) {
    emit S_Message(msg);
}

void Handler::onSearchUserResponse(const std::vector<User>& users) {
    emit S_UserSearch(users);
}

void Handler::onErrorMessage(const std::string& text) {
    std::cerr << text << '\n';
}

void Handler::onHistoryResponse(const bool success, const int64_t dialog_id, const std::vector<Message>& messages, const std::string& reason) {
    if(success)
        emit S_HistoryLoaded(dialog_id, messages);
    else
        onErrorMessage(reason);
}

void Handler::onGetDialogsResponse(const bool success, const std::vector<MetaDialog_Client>& dialogs, const std::string& reason) {
    if(success)
        emit S_DialogsLoaded(dialogs);
    else
        onErrorMessage(reason);
}

void Handler::onResumeConnectionResponse(const bool success) {
    if(success)
        emit S_ConnectionSucess();
}
