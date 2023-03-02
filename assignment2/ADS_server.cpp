#include "ADS_server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 1024

ADS::ADS(vector<pair<string, string>> _ip) {
  int i = 1;
  for (auto v : _ip) {
    ip[v.first] = v.second;
  }
}

string ADS::handle(char *req) {
  string word(req);
  if (ip.find(word) != ip.end()) {
    return ip[word];
  } else
    return "Error:ADS:404";
}

void ADS::server(int PORT) {
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

  while(true) {
  // recieves request from NR
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
                (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';

    string res = handle(buffer);

    sendto(sockfd, res.c_str(), res.size(), MSG_CONFIRM,
          (const struct sockaddr *)&cliaddr, len);
  }
}