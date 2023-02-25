// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 2
// Date of submission: <fill>
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://people.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <fstream>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

using namespace std;

int PORT;
string filename;

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("Error: Wrong number of given arguments");
  }

  PORT = atoi(argv[1]);
  filename = argv[2];

  return 0;
}