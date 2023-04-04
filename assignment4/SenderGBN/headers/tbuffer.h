// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 4
// Date of submission: 04-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://www.geeksforgeeks.org/udp-server-client-implementation-c/

#ifndef TBUFFER_H
#define TBUFFER_H

#include <bits/stdc++.h>
#include "packet.h" 

using namespace std;

class T_Buffer {
public:
    vector<PACKET> buffer;
    vector<chrono::microseconds> time_generated;
    int max_size;
    //tells if buffer is free to fill
    bool is_free() {
        if(buffer.size() < max_size) return true;
        return false;
    }
    //add into the buffer
    void add(PACKET p) {
        buffer.push_back(p);
    }
};

#endif