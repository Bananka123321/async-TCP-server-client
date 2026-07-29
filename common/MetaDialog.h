#pragma once

#include <string>
#include <vector>
#include "Message.h"

struct MetaDialog{
    int peer_id;
    std::string username;
    std::string last_msg_preview;
    int64_t last_msg_timestamp;
};