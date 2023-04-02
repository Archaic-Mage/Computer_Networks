#ifndef CLIENT_H
#define CLIENT_H

#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

class Client {
    int sockfd;
    struct sockaddr_in servaddr;
public:
    Client(string, int);
    string recieve();
    void send(string msg);
};


#endif