#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 1024

using namespace std;

void _client(int PORT, string IP) {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;
   
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&servaddr, 0, sizeof(servaddr));
       
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT+53);
    servaddr.sin_addr.s_addr = inet_addr(IP.c_str());
       
    int n;
    socklen_t len;

    string url_name;
    cout << "Enter Server's name: ";
    cin >> url_name;
       
    sendto(sockfd, url_name.c_str(), url_name.size(),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
            sizeof(servaddr));
           
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, (struct sockaddr *) &servaddr,
                &len);
    buffer[n] = '\0';

    cout << "The IP Address: " + string(buffer) << endl;
   
    close(sockfd);
}