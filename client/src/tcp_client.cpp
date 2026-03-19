#include "../include/tcp_client.h"

TCPClient::TCPClient(int port) : port(port), clientSocket(-1) {}

TCPClient::~TCPClient() {
    if (clientSocket != -1) close(clientSocket);    
}

bool TCPClient::start() {
    if(!setupSocket()) return false;
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
  
    std::cout << "You successfully connected to!!\n";
    return true;    
}


void TCPClient::run() {
    char buffer[16384];
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << buffer << std::endl;

    while (1) {
        std::cin >> buffer;
        std::cout << std::endl;
        send(clientSocket, buffer, strlen(buffer), 0);
    }
    
}