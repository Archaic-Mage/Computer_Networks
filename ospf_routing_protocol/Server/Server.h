// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 5
// Date of submission: 28-04-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://www.geeksforgeeks.org/udp-server-client-implementation-c/

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
    void send(string msg, int PORT, string IP);
};

#endif