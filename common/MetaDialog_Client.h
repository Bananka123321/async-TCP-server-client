#pragma once

#include <string>

struct MetaDialog_Client{
    int64_t dialog_id;
    int64_t last_msg_timestamp;
    std::string username;
    std::string display_name;
    std::string last_msg_preview;
    int unread_count;
    int peer_id;
};