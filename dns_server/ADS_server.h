// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 2
// Date of submission: 03-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://people.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html

#ifndef ADS_server
#define ADS_server

#include <bits/stdc++.h>

using namespace std;


class ADS {
  unordered_map<string, string> ip;
  string _type;
  public:
    ADS(vector<pair<string, string>>, string);
    string handle(char *req);
    void server(int);
    void log(int, string, string);
};

#endif