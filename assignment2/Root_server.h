// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 2
// Date of submission: 03-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://people.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html

#ifndef Root_server
#define Root_server

#include <bits/stdc++.h>

using namespace std;


class RDS {
  // maps .com/.edu -> IP of TLD
   unordered_map<string, string> tld_ip;
  public:
    RDS(string, string, int);
     string handle(char *req);
     void server(int);
     void log(int, string, string);
     pair<string,int> extract(string);
     void send_bye(int);
};

#endif