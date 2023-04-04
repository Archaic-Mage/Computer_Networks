// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 4
// Date of submission: 04-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://www.geeksforgeeks.org/udp-server-client-implementation-c/

#ifndef PACKET_H
#define PACKET_H

#include <bits/stdc++.h>

using namespace std;

class PACKET {
public:
    char SEQ;
    string message;
    
    friend PACKET unpack(string);
    string stringify() {
        string ret ="";
        ret += SEQ;
        for(auto c: message) {
            ret += c;
        }

        return ret;
    }
};

PACKET unpack(string s) {
    PACKET packet = PACKET();
    packet.SEQ = s[0];
    packet.message = "";

    for(int i = 1; i<s.size(); i++) packet.message += s[i];

    return packet;
}

#endif