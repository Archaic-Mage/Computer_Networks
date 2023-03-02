// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 2
// Date of submission: 03-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://people.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html

#ifndef NR_server
#define NR_server

#include <bits/stdc++.h>

using namespace std;


class NR {
   string root;
   string name_requested;
  public:
    NR(string);
    pair<string, int> extract(string);
    string request(string, int, int);
    void server(int);
    void log(int, string, string);
};

#endif