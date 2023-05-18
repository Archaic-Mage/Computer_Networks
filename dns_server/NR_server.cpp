// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 2
// Date of submission: 03-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://people.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html

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

//stores root IP:PORT information
NR::NR(string IP) { 
  root = IP;
}

//stores log information
void NR::log(int type, string data, string ip_port) {
  ofstream file("./log_files/NR.output", ios::app);
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
    file << "Stopping the server."<< endl;
  }

  file.close();
}

//Given IP:PORT input we extract the information of IP and PORT
//ie. 127.0.0.1:8080 -> {127.0.0.1 , 8080}
pair<string,int> NR::extract(string IP_PORT) {
  stringstream ss(IP_PORT);
  string ip;
  string PORT;
  getline(ss, ip, ':');
  getline(ss, PORT, ':');
  return {ip, stoi(PORT)};    
}

//sends the request to the given 'IP', on 'PORT', using socket 'sockfd'
//returns the response of the request
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
    //requests the data
    log(0, name_requested, IP + ":" + to_string(PORT));
    sendto(sockfd, name_requested.c_str(), name_requested.length(),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
            sizeof(servaddr));
    //recieves the response     
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, (struct sockaddr *) &servaddr,
                &len);
    buffer[n] = '\0';
    log(1, string(buffer), IP + ":" + to_string(PORT));

    return string(buffer);
}
//starts the server on 'PORT'
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

  while(true) {
    // recieves request from client
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
                (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';

    name_requested = string(buffer);
    //log info
    log(-1, name_requested, string(inet_ntoa(cliaddr.sin_addr)) + ":" + to_string(ntohs(cliaddr.sin_port)));

    if(name_requested == "bye") {
      log(0, name_requested, root + ":" + to_string(PORT+1));
      cliaddr.sin_addr.s_addr = inet_addr(root.c_str());
      cliaddr.sin_port = htons(PORT+1);
      n = sendto(sockfd, name_requested.c_str(), name_requested.size(), MSG_CONFIRM,
            (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
      break;
    }

    string response;
    pair<string, int> IP_PORT;

    //requests the root server for tld server 
    response = request(root, PORT+1, sockfd); 
    //if record not present
    if(response == "Error:RDS:404") {
      response = "Error:NR:404";
      log(-2, response, string(inet_ntoa(cliaddr.sin_addr)) + ":" + to_string(ntohs(cliaddr.sin_port)));
      n = sendto(sockfd, response.c_str(), response.size(), MSG_CONFIRM,
            (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
      continue;
    }   
    IP_PORT = extract(response);  

    //request the corresponding TLD server for ADS 
    response = request(IP_PORT.first, IP_PORT.second, sockfd);
    if(response == "Error:TLD:404") {
      response = "Error:NR:404";
      log(-2, response, string(inet_ntoa(cliaddr.sin_addr)) + ":" + to_string(ntohs(cliaddr.sin_port)));
      n = sendto(sockfd, response.c_str(), response.size(), MSG_CONFIRM,
            (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
      continue;
    } 
    IP_PORT = extract(response);

    //request the ADS for IP address
    response = request(IP_PORT.first, IP_PORT.second, sockfd);
    if(response == "Error:ADS:404") {
      response = "Error:NR:404";
      log(-2, response, string(inet_ntoa(cliaddr.sin_addr)) + ":" + to_string(ntohs(cliaddr.sin_port)));
      n = sendto(sockfd, response.c_str(), response.size(), MSG_CONFIRM,
            (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
      continue;
    } 

    log(-2, response, string(inet_ntoa(cliaddr.sin_addr)) + ":" + to_string(ntohs(cliaddr.sin_port)));

    n = sendto(sockfd, response.c_str(), response.size(), MSG_CONFIRM,
            (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
  }

  close(sockfd);
}