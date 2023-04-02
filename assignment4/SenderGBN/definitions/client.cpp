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
}

string Client::recieve() {
    socklen_t len;
    int n;
    char buffer[100000];
   
    len = sizeof(servaddr);  //len is value/result
   
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &servaddr,
                &len);
    buffer[n] = '\0';

    return string(buffer);
}

void Client::send(string msg) {
    socklen_t len;
    len = sizeof(servaddr);
    
    sendto(sockfd, msg.c_str(), msg.length(), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,
            len);
}