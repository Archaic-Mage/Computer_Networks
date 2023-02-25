#include "NR_server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 1024


NR::NR(string IP) { root = IP; }

pair<string,int> NR::extract(string IP_PORT) {
  stringstream ss(IP_PORT);
  string ip;
  string PORT;
  getline(ss, ip, ':');
  getline(ss, PORT, ':');
  return {ip, stoi(PORT)};    
}

string NR::request(string IP, int PORT, int sockfd) {
    char buffer[MAXLINE];
    struct sockaddr_in     servaddr;
   
    memset(&servaddr, 0, sizeof(servaddr));
       
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(IP.c_str());
       
    int n;
    socklen_t len;
       
    sendto(sockfd, name_requested.c_str(), name_requested.length(),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
            sizeof(servaddr));
           
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, (struct sockaddr *) &servaddr,
                &len);
    buffer[n] = '\0';

    return string(buffer);
}

void NR::server(int PORT) {
  int sockfd;
  struct sockaddr_in servaddr, cliaddr;
  char buffer[MAXLINE];

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  socklen_t len;
  int n;

  len = sizeof(cliaddr); // len is value/result

  // recieves request from client
  n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
               (struct sockaddr *)&cliaddr, &len);
  buffer[n] = '\0';

  name_requested = string(buffer);
  string response;
  pair<string, int> IP_PORT;

  //requests the root server for tld server 
  response = request(root, PORT+1, sockfd);
  IP_PORT = extract(response);

  //request the corresponding TLD server for ADS 
  response = request(IP_PORT.first, IP_PORT.second, sockfd);
  IP_PORT = extract(response);

  //request the ADS for IP address
  response = request(IP_PORT.first, IP_PORT.second, sockfd);
  
  n = sendto(sockfd, response.c_str(), response.size(), MSG_CONFIRM,
          (const struct sockaddr *) &cliaddr, sizeof(cliaddr));

  close(sockfd);
}