// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 4
// Date of submission: 04-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://www.geeksforgeeks.org/udp-server-client-implementation-c/

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
    int sock;
    struct sockaddr_in servaddr;
public:
    Client(string, int);
    void set_timeout(int T);
    string recieve();
    void send(string msg);
};


#endif