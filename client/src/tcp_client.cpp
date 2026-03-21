#include "../include/tcp_client.h"

TCPClient::TCPClient(int port) : port(port), clientSocket(-1) {}

TCPClient::~TCPClient() {
    if (clientSocket != -1) close(clientSocket);    
}

bool TCPClient::start() {
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
            if (!recvPacket(clientSocket, msg)) break;
            
            j = json::parse(msg);
            type = j["type"];
            
            if (type == "broadcastMessage")
                std::cout << "[" << j["from"] << "] " << j["text"] << std::endl;
            
                else if (type == "privateMessage")
                std::cout << "(private) [" << j["from"] << "] " << j["text"] << std::endl;
        }
    });

    std::thread write([this]() {
        char text[16384];
        while (true) {
            std::cin.getline(text, sizeof(text));
            std::string message, to, body;

            //private message will started with #
            //broadcast message will started without #

            if (text[0] == '#') {
                int space = std::string(text).find_first_of(' ');
                if (space != std::string::npos) {
                    to = std::string(text).substr(1, space - 1);
                    body = std::string(text).substr(space + 1);
                    message  = protocol::privateMessage(login, to, body);
                } else {
                    std::cout << "WRONG PROTOCOL FORMAT";
                    continue; //wrong format
                }
            } else {
                message = protocol::broadcastMessage(login, text);
            }

            if (!sendPacket(clientSocket, message)) break;
        }
    });

    read.join();
    write.join();    
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