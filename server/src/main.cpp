#include "../include/tcp_server.h"

int main() {
    TCPServer server(5555);
    if (!server.start()) {
        std::cerr << "Server failed to start\n";
        return 1;
    }
    return 0;
}