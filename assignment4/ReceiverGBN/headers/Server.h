#ifndef Server_f
#define Server_f

#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

class Server {

    int sockfd;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;

    //functions for creating server, sending and recieving data packets
public:

    //constructor function to start the server
    Server(int);
    //get sockid
    void get_sockfd() {
        cout << sockfd << endl;
    }
    //helper function to recieve data from clients
    string recieve();
    //helper function to send the data to clients
    void send(string msg);
};

#endif