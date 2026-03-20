#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <thread>
#include <mutex>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class TCPServer
{
public:
    TCPServer(int port);
    ~TCPServer();

    bool start();

private:
    int port;
    int serverSocket;
    std::vector<int> clients;
    std::mutex clientsMutex;

    bool setupSocket();
    void run();
    bool clientHandle(int sock);
    void clientDisconnect(int clientSocket);
};