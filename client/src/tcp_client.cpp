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
    send(clientSocket, msg.c_str(), msg.size(), 0);
    std::cout << "You successfully connected to!!\n";
    return true;    
}

void TCPClient::run() {
    std::thread read([this]() {
        char buffer[16384];
        json j;
        std::string type;
        while (true) {
            int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0) break;
            buffer[bytes] = '\0';

            j = json::parse(buffer);
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
                    // std::cout << login << ' ' << to << ' ' << body << std::endl;
                } else {
                    std::cout << "WRONG PROTOCOL FORMAT";
                    continue; //wrong format
                }
            } else {
                message = protocol::broadcastMessage(text);
            }

            if (send(clientSocket, message.c_str(), message.size(), 0) <= 0) break;
        }
    });

    read.join();
    write.join();    
}