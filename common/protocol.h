#pragma once

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace protocol {
    inline std::string broadcastMessage(const std::string& sender, const std::string& text) {
        json j;
        j["type"] = "broadcastMessage";
        j["from"] = sender;
        j["text"] = text;
        return j.dump();
    }

    inline std::string makeLogin(const std::string& user) {
        json j;
        j["type"] = "login";
        j["user"] = user;
        return j.dump();
    }

    inline std::string privateMessage(const std::string& sender, const std::string& user, const std::string& text) {
        json j;
        j["type"] = "privateMessage";
        j["text"] = text;
        j["from"] = sender;
        j["to"] = user;
        return j.dump();
    }
}