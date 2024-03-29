// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 2
// Date of submission: 03-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://people.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html

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

//client
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

    while(true) {
        //asks for server name
        string url_name;
        cout << "Enter Server Name: ";
        cin >> url_name;
        
        //sends the request to NR server
        sendto(sockfd, url_name.c_str(), url_name.size(),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
                sizeof(servaddr));
        
        if(url_name == "bye") break;
        
        //The NR server gives the server name or sends Error:NR:404 - meaning DNS mapping not found
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);
        buffer[n] = '\0';

        string response = (string) buffer;

        if(response == "Error:NR:404") {
            cout << "No DNS Record Found" << endl;
        } else {
        cout << "DNS Mapping: " + string(buffer) << endl;
        }
    }
   
    close(sockfd);
}