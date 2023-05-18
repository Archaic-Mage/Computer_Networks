// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 4
// Date of submission: 04-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://www.geeksforgeeks.org/udp-server-client-implementation-c/

#ifndef HDEBUG
#define HDEBUG

#include <iostream>
#include <vector>
using namespace std;

bool DEBUG = false;

//prints the value if DEBUG is set to true
void debug(auto x) {
    if(!DEBUG) return;
    cout << x;
}


#endif