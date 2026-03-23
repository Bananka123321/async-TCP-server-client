#pragma once
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET Socket;
#pragma comment(lib, "Ws2_32.lib")
#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int Socket;

#endif

#include <iostream>
#include <cstring>
#include <thread>
#include <functional>

#include "protocol.h"

class TCPClient
{
public:
    TCPClient(int port);
    ~TCPClient();

    bool start();

    void sendMessage(const std::string& msg);
    std::function<void(const std::string&)> onMessage;
    void setLogin(const std::string& l);

private:
    int port;
    int clientSocket;

    std::string login;

    bool setupSocket();
    void run();
    bool sendAll(int sock, const void* data, size_t size);
    bool sendPacket(int sock, const std::string& data);
    bool recvAll(int sock, void* buffer, size_t size);
    bool recvPacket(int sock, std::string &data);

};
