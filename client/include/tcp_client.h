#pragma once

#include <iostream>
#include <cstring>
#include <thread>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../../common/protocol.h"

class TCPClient
{
public:
    TCPClient(int port);
    ~TCPClient();

    bool start();

private:
    int port;
    int clientSocket;

    std::string login = "Ivan";

    bool setupSocket();
    void run();
    bool sendAll(int sock, const void* data, size_t size);
    bool sendPacket(int sock, const std::string& data);
    bool recvAll(int sock, void* buffer, size_t size);
    bool recvPacket(int sock, std::string &data);
};