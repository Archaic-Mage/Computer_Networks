#include "Root_server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 1024

RDS::RDS(string com, string edu, int INIT_PORT) {
  tld_ip["com"] = com + ":" + to_string(INIT_PORT + 1);
  tld_ip["edu"] = edu + ":" + to_string(INIT_PORT + 2);
}

string RDS::handle(char *req) {
  stringstream ss(req);
  string word;
  while (!ss.eof()) {
    getline(ss, word, '.');
  }

  if (tld_ip.find(word) != tld_ip.end()) {
    return tld_ip[word];
  } else
    return "Error:RDS:404";
}

void RDS::server(int PORT) {
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

  // recieves request from NR
  n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
               (struct sockaddr *)&cliaddr, &len);
  buffer[n] = '\0';

  string res = handle(buffer);

  sendto(sockfd, res.c_str(), res.size(), MSG_CONFIRM,
         (const struct sockaddr *)&cliaddr, len);
}