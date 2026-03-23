#include "../include/tcp_client.h"

TCPClient::TCPClient(int port) : port(port), clientSocket(-1) {}

TCPClient::~TCPClient() {
    if (clientSocket != -1) {
        #ifdef _WIN32
                closesocket(clientSocket);
        #else
                close(clientSocket);
        #endif
    }
}

bool TCPClient::start() {
    #ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2,2), &wsa);
    #endif
    if (!setupSocket()) return false;
    run();

    return true;
}

bool TCPClient::setupSocket() {
    clientSocket = socket(AF_INET, SOCK_STREAM ,IPPROTO_TCP);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket\n";
        return false;
    }
    
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Incorrect IP ADRESS\n";
        return false;
    }
    
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Connect failed\n";
        return false;
    }
    
    std::string msg = protocol::makeLogin(login);
    if (!sendPacket(clientSocket, msg)) {
        std::cerr << "Failed to set login\n";
        return false;
    }
    std::cout << "You successfully connected to!!\n";
    return true;    
}

void TCPClient::run() {
    std::thread read([this]() {
        json j;
        std::string type;
        std::string msg;
        while (true) {
            if (!recvPacket(clientSocket, msg)){
                std::cout << "SERVER IS NOT RUNNING" << std::endl;
                break;
            }
            j = json::parse(msg);
            if (onMessage) onMessage(msg);
        }
    });
    read.join();  
}

bool TCPClient::sendAll(int sock, const void* data, size_t size) {
    size_t total = 0;
    int sent;
    while (total < size) {
        sent = send(sock, (char*)data + total, size - total, 0);
        if (sent <= 0) return false;
        total += sent;
    }

    return true;
}

bool TCPClient::sendPacket(int sock, const std::string& data) {
    uint32_t len = htonl(data.size());

    if (!sendAll(sock, &len, sizeof(len)))
        return false;

    if (!sendAll(sock, data.data(), data.size()))
        return false;

    return true;
}

bool TCPClient::recvAll(int sock, void* data, size_t size) {
    size_t total = 0;
    int bytes;
    while (total < size) {
        bytes = recv(sock, (char*)data + total, size - total, 0);
        if (bytes <= 0) return false;
        total += bytes;
    }

    return true;
}

bool TCPClient::recvPacket(int sock, std::string &data) {
    uint32_t len;
    if (!recvAll(sock, &len, sizeof(len))) return false;

    len = ntohl(len);
    data.resize(len);

    if (!recvAll(sock, data.data(), len)) return false;
    return true;
}

void TCPClient::sendMessage(const std::string& msg) {
    sendPacket(clientSocket, msg);
}

void TCPClient::setLogin(const std::string& l) {
    login = l;
}
