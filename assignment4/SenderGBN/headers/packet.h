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