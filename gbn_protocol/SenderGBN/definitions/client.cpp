// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 4
// Date of submission: 04-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://www.geeksforgeeks.org/udp-server-client-implementation-c/

#include "../headers/client.h"
#include <bits/stdc++.h>

using namespace std;

int MAXLINE = 1024;

Client::Client(string ip, int PORT) {
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&servaddr, 0, sizeof(servaddr));
       
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sock = sockfd;
}

void Client::set_timeout(int TIMEOUT) {
    // set the socket timeout
    sock = sockfd;
    struct timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = TIMEOUT;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        std::cerr << "Failed to set socket timeout" << std::endl;
        exit(1);
    }
}

string Client::recieve() {
    socklen_t len;
    size_t n;
    char buffer[100000];
   
    len = sizeof(servaddr);  //len is value/result
   
    n = recvfrom(sock, (char *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &servaddr,
                &len);

    string ret = "";
    if(n != 4) {
        return ret;
    }
    buffer[n] = '\0';
    ret += buffer[0];

    return ret;
}

void Client::send(string msg) {
    socklen_t len;
    len = sizeof(servaddr);
    
    sendto(sockfd, msg.c_str(), msg.length(), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,
            len);
}