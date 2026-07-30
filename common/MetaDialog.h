#pragma once

#include <string>
#include <vector>
#include "Message.h"

struct MetaDialog{
    int64_t dialog_id;
    int peer_id;
    std::string username;
    std::string display_name;
    std::string last_msg_preview;
    int64_t last_msg_timestamp;
    int unread_count;
};