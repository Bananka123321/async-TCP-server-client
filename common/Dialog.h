#pragma once

#include <string>
#include <vector>

#include "Message.h"

struct Dialog {
    std::vector<Message> messages;
    int peerId;
};
