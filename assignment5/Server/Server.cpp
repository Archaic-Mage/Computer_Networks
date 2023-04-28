// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 5
// Date of submission: 28-04-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://www.geeksforgeeks.org/udp-server-client-implementation-c/

#include "Server.h"
#include <bits/stdc++.h>

using namespace std;

int MAXLINE = 1024;

Server::Server(int PORT) {
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
       
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE); 
    }
}

string Server::recieve() {
    socklen_t len;
    int n;
    char buffer[100000];
   
    len = sizeof(cliaddr);  //len is value/result
   
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';

    return string(buffer);
}

void Server::send(string msg, int PORT, string IP) {
    socklen_t len;
    len = sizeof(cliaddr);

    //sets cliaddr info
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(PORT);
    cliaddr.sin_addr.s_addr = inet_addr(IP.c_str());
    
    sendto(sockfd, msg.c_str(), msg.length(), 
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
}