#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <thread>
#include <mutex>
#include <algorithm>
#include <unordered_map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../../common/protocol.h"

class TCPServer
{
public:
    TCPServer(int port);
    ~TCPServer();

    bool start();

private:
    int port;
    int serverSocket;
    std::vector<int> clients; //Mb using later
    std::unordered_map<std::string, int> userToSocket;
    std::unordered_map<int, std::string> socketToUser;
    std::mutex clientsMutex;
    std::mutex usersMutex;

    bool setupSocket();
    void run();
    bool clientHandle(int sock);
    void clientDisconnect(int clientSocket);
    bool sendAll(int sock, const void* data, size_t size);
    bool sendPacket(int sock, const std::string& data);
    bool recvAll(int sock, void* buffer, size_t size);
    bool recvPacket(int sock, std::string &data);
};