#pragma once

#include <string>
#include <QByteArray>
#include <QtEndian>
#include <optional>
#include <cstring>

constexpr uint32_t MAX_PACKET_SIZE = 1024 * 1024;

class PacketIO_Client {
public:
    static std::optional<std::string> extractPacket(QByteArray& buffer);
};