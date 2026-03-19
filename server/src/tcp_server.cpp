#include "../include/tcp_server.h"

TCPServer::TCPServer(int port) : port(port), serverSocket(-1) {}

TCPServer::~TCPServer() {
    if (serverSocket != -1) close(serverSocket);
}

bool TCPServer::start() {
    if (!setupSocket()) return false;
    run();

    return true;
}

bool TCPServer::setupSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket\n";
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Bind failed\n";
        return false;
    }

    if (listen(serverSocket, 3) == -1) { //Later replace 3 with SOMAXCONN
        std::cerr << "Listen failed\n";
        return false;
    }

    std::cout << "Server listening on port " << port << std::endl;
    return true;
}

void TCPServer::run() {
    char buffer[16384];
    int bytes_received;
    while (true) {
        sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, (socklen_t*)&addrLen);

        if (clientSocket == -1)
            continue;

        std::cout << "CLIENT CONNECTED\n";
        char *msg = "Hello!!! Type each other anything:\n";
        send(clientSocket, msg, strlen(msg), 0);
        while (1) {   
            bytes_received = recv(clientSocket, buffer, sizeof(buffer), 0);
            
            if (bytes_received <= 0) continue;
            buffer[bytes_received] = '\0';
            
            std::cout << buffer << std::endl;
        }
    }
}