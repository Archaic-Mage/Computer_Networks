#include <thread>
#include <bits/stdc++.h>
#include <chrono>
#include <random>

#include "headers/Server.h"
#include "./headers/debug_print.h"

using namespace std;

int RANDOM_DROP_RATE = 1;
int PORT = 0;
int MAX_PACKETS = 0;
char NES = 0;

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

bool will_drop() {
    mt19937 gen{random_device{}()};
    
    // Create a uniform distribution over the range [0.0, 1.0)
    uniform_real_distribution<double> dist{0.0, 1.0};

    //generate a random number
    double prob_assigned = dist(gen);

    if(prob_assigned < RANDOM_DROP_RATE) {
        return true;
    }

    return false;
}

void handle_args(int argc, char** argv) {
    if(argc < 7) {
        cout << "Wrong number of arguments" << endl;
        exit(1);
    }

    for(int i = 1; i<argc; i++) {
        string option = string(argv[i]);
        if(option == "-d") {
            DEBUG = true;
            continue;
        }
        i++;
        int value = atoi(argv[i]);
        if(option == "-p") {
            PORT = value;
        } else if(option == "-n") {
            MAX_PACKETS = value;
        } else if(option == "-e") {
            RANDOM_DROP_RATE = value;
        }
    }
}

void process_packet(auto start, string req, Server reciever) {
    string drop = "false";
    bool dropped = will_drop();
    if(dropped) drop = "true";

    //timing details
    auto now = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::microseconds>(now - start);
    auto milliseconds = elapsed.count() / 1000;
    auto microseconds = elapsed.count() % 1000;

    //debug info
    //seq
    debug("Seq ");
    debug(int(req[0]));
    //time recieved
    debug(": Time Recieved: ");
    debug(milliseconds);
    debug(":");
    debug(microseconds);
    //packet status
    debug(" Packet dropped: ");
    debug(drop);
    debug("\n");
    
    if(dropped) return;

    PACKET packet = unpack(req);

    if(packet.SEQ != NES) return;
    
    NES++;

    string ack = "";
    ack += packet.SEQ;

    reciever.send(ack);
    MAX_PACKETS--;
}

void sum(int a , int b) {
    cout << a+b << endl;
}

int main() {

    thread t1 = thread(sum, 5,6);
    t1.join();
    return 0;
}