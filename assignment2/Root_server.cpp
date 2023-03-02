// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 2
// Date of submission: 03-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://people.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html

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

//stores log information
void RDS::log(int type, string data, string ip_port) {
  ofstream file("./log_files/RDS.output", ios::app);
  if(type == 0) {
    file << "REQ to " << ip_port << " info: " << data << endl;
  } else if(type == 1) {
    file << "RES from " << ip_port << " data: " << data << endl;
  } else if(type == -1) {
    file << "REQ from " << ip_port << " info: " << data << endl;
  } else if(type == -2) {
    file << "RES sent " << ip_port << " data: " << data << endl;
  }

  if(data == "bye" && type == 0) {
    file << "Stopping server." << endl;
  }
  file.close();
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

pair<string,int> RDS::extract(string IP_PORT) {
  stringstream ss(IP_PORT);
  string ip;
  string PORT;
  getline(ss, ip, ':');
  getline(ss, PORT, ':');
  return {ip, stoi(PORT)};    
}

void RDS::send_bye(int fd) {
  struct sockaddr_in cliaddr;
  int len = sizeof(cliaddr);

  cliaddr.sin_family = AF_INET;

  pair<string, int> ip_port;
  for(auto x: tld_ip) {
    ip_port = extract(x.second);
    cliaddr.sin_addr.s_addr = inet_addr(ip_port.first.c_str());
    cliaddr.sin_port = htons(ip_port.second);
    //log info
    log(0, "bye", x.second);
    string res = "bye";
    sendto(fd, res.c_str(), res.size(), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
  }
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
  while(true) {
    // recieves request from NR
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
                (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    //log info
    log(-1, string(buffer), string(inet_ntoa(cliaddr.sin_addr)) + ":" + to_string(ntohs(cliaddr.sin_port)));

    if(string(buffer) == "bye") {
      send_bye(sockfd);
      break;
    }

    string res = handle(buffer);

    //log info
    log(-2, res, string(inet_ntoa(cliaddr.sin_addr)) + ":" + to_string(ntohs(cliaddr.sin_port)));
    sendto(sockfd, res.c_str(), res.size(), MSG_CONFIRM,
          (const struct sockaddr *)&cliaddr, len);
  }

  close(sockfd);
  
}