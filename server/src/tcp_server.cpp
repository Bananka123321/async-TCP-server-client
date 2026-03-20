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

    if (listen(serverSocket, SOMAXCONN) == -1) { 
        std::cerr << "Listen failed\n";
        return false;
    }

    std::cout << "Server listening on port " << port << std::endl;
    return true;
}

void TCPServer::run() {
    while (true) {
        sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, (socklen_t*)&addrLen);

        if (clientSocket == -1)
            continue;

        std::cout << "CLIENT CONNECTED\n";
        const char *msg = "HELLO YOU: \n";
        if (send(clientSocket, msg, strlen(msg), 0) == -1) {
            std::cerr << "Ny tyt nado chto-to delat posle raboti s bd\n";
            clientDisconnect(clientSocket);
        }
        
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(clientSocket);
        }

        std::thread([this, clientSocket](){
            bool alive = clientHandle(clientSocket);
            clientDisconnect(clientSocket);
        }).detach();
    }
}

bool TCPServer::clientHandle(int sock) {
    char buffer[16384];
    int bytes_received;
    std::vector<int> clientsCopy;
    while (true) {
        bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        
        if (bytes_received == 0){
            std::cout << "You was disconnected from server...\n";
            return true;
        } else if (bytes_received == -1) {
            std::cerr << "Poka hz che proizoshlo...\n";
            return false;
        }

        buffer[bytes_received] = '\0';
        std::cout << buffer << std::endl;

        {    
            std::lock_guard<std::mutex> lock(clientsMutex);
            clientsCopy = clients;
        }

        for (int socket : clientsCopy) {
            if (socket == sock) continue;
            if (send(socket, buffer, bytes_received, 0) == -1) {
                std::cerr << "Ny tyt nado chto-to delat posle raboti s bd\n";
                clientDisconnect(socket);
            };
        }
    }
}

void TCPServer::clientDisconnect(int clientSocket) {
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    }
    close(clientSocket);
    std::cout << "CLIENT WAS DISCONNECTED!\n";
}