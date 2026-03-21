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

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed\n";
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
    std::vector<int> clientsCopy;
    json j;
    std::string msgType;
    std::string msg;
    while (true) {
        if (!recvPacket(sock,msg)) {
            clientDisconnect(sock);
            return false;
        }
        
        j = json::parse(msg);
        msgType = j["type"];
        if (msgType == "login") { //user was logging
            {
                std::lock_guard<std::mutex> lock(usersMutex);
                socketToUser[sock] = j["user"];
                userToSocket[j["user"]] = sock;
            }
        } else if (msgType == "broadcastMessage") { //user send message to each other clients
            {    
            std::lock_guard<std::mutex> lock(clientsMutex);
            clientsCopy = clients;
            }

            for (int socket : clientsCopy) {
                if (socket == sock) continue;
                if (!sendPacket(socket, msg)) {
                    std::cerr << "Ny tyt nado chto-to delat posle raboti s bd\n";
                    clientDisconnect(socket);
                };
            }
        } else if (msgType == "privateMessage") {
            std::lock_guard<std::mutex> lock(usersMutex);
            
            std::string toUser = j["to"];
            auto it = userToSocket.find(toUser);
            
            if (it == userToSocket.end()) continue;

            if (!sendPacket(it->second, msg)) {
                std::cerr << "Ny tyt nado chto-to delat posle raboti s bd\n";
                clientDisconnect(it->second);
            };
        }
    

        //LOGS
        if (j["type"] == "privateMessage")
            std::cout << "[" << j["from"] << "] to [" << j["to"] << "] " << j["text"]<< std::endl;//message log
        else if (j["type"] == "broadcastMessage")
            std::cout << "[" << socketToUser[sock] << "] " << j["text"]<< std::endl;//message log
    }
}

void TCPServer::clientDisconnect(int clientSocket) {
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    }
    {
        std::lock_guard<std::mutex> lock(usersMutex);
        auto it = socketToUser.find(clientSocket);
        if (it != socketToUser.end()) {
            userToSocket.erase(it->second);
            socketToUser.erase(it);
        }
    }

    close(clientSocket);
    std::cout << "CLIENT WAS DISCONNECTED!\n";
}

bool TCPServer::sendAll(int sock, const void* data, size_t size) {
    size_t total = 0;
    int sent;
    while (total < size) {
        sent = send(sock, (char*)data + total, size - total, 0);
        if (sent <= 0) return false;
        total += sent;
    }

    return true;
}

bool TCPServer::sendPacket(int sock, const std::string& data) {
    uint32_t len = htonl(data.size());

    if (!sendAll(sock, &len, sizeof(len)))
        return false;

    if (!sendAll(sock, data.data(), data.size()))
        return false;

    return true;
}

bool TCPServer::recvAll(int sock, void* data, size_t size) {
    size_t total = 0;
    int bytes;
    while (total < size) {
        bytes = recv(sock, (char*)data + total, size - total, 0);
        if (bytes <= 0) return false;
        total += bytes;
    }

    return true;
}

bool TCPServer::recvPacket(int sock, std::string &data) {
    uint32_t len;
    if (!recvAll(sock, &len, sizeof(len))) return false;

    len = ntohl(len);
    data.resize(len);

    if (!recvAll(sock, data.data(), len)) return false;
    return true;
}