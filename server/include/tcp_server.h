#pragma once

#include <iostream>
#include <cstring>
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

    bool setupSocket();
    void run();
};