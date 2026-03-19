#include "../include/tcp_client.h"

int main() {
    TCPClient Client(5555);
    if (!Client.start()) {
        std::cerr << "Client failed to start\n";
        return 1;
    }
    return 0;
}