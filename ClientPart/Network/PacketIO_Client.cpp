#include "PacketIO_Client.h"

std::optional<std::string> PacketIO_Client::extractPacket(QByteArray& buffer) {
    if (buffer.size() < 4) {
        return std::nullopt;
    }

    uint32_t len_net;
    std::memcpy(&len_net, buffer.constData(), 4);
    uint32_t len = qFromBigEndian(len_net);

    if (len == 0 || len > MAX_PACKET_SIZE) {
        buffer.clear();
        return std::nullopt;
    }

    if (buffer.size() < 4 + static_cast<int>(len)) {
        return std::nullopt;
    }

    std::string result(buffer.constData() + 4, len);

    buffer.remove(0, 4 + len);

    return result;
}