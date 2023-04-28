// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 4
// Date of submission: 04-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://www.geeksforgeeks.org/udp-server-client-implementation-c/

#include <thread>
#include <random>
#include <chrono>
#include <bits/stdc++.h>

#include "./headers/Server.h"
#include "./headers/debug_print.h"
#include "./headers/packet.h"

using namespace std;

#define _time_t chrono::_V2::system_clock::time_point

double RANDOM_DROP_RATE = 1;
int PORT = 0;
int MAX_PACKETS = 0;
char NES = 0;
int recieved_packet= 0;
int ack_sent = 0;

vector<thread*> threads;

void debug2(auto x) {
    return;
    cout << x << endl;
}

bool will_drop() {
    mt19937 gen{random_device{}()};
    
    // Create a uniform distribution over the range [0.0, 1.0)
    uniform_real_distribution<double> dist{0.0, 1.0};

    //generate a random number
    double prob_assigned = dist(gen);

    //debug
    debug2(prob_assigned);
    debug2(RANDOM_DROP_RATE);

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
        if(option == "-p") {
            int value = atoi(argv[i]);
            PORT = value;
        } else if(option == "-n") {
            int value = atoi(argv[i]);
            MAX_PACKETS = value;
        } else if(option == "-e") {
            //debug
            debug2(argv[i]);
            debug2(string(argv[i]));
            debug2(stod(string(argv[i])));
            RANDOM_DROP_RATE = stod(string(argv[i]));
        }
    }

    //debug
    debug2("done handling");
}

void process_packet(_time_t start, string req, Server reciever) {

    string drop = "false";
    bool dropped = will_drop();
    if(dropped) drop = "true";
    debug2("started the thread");
    //timing details
    _time_t now = chrono::high_resolution_clock::now();
    chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
    int milliseconds = elapsed.count() / 1000;
    int microseconds = elapsed.count() % 1000;

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
    if(NES < 0) NES+=128;

    string ack = "";
    ack += packet.SEQ;
    ack += "ACK";

    reciever.send(ack);
    debug2("Sent ACK");
    debug2(int(ack[0]));
    
    ack_sent++;
    MAX_PACKETS--;
}


int main(int argc, char** argv) {

    handle_args(argc, argv);

    Server reciever = Server(PORT);
    _time_t start = chrono::high_resolution_clock::now();

    while(MAX_PACKETS) {
        //debug
        debug2("Started while loop");
        string req = reciever.recieve();
        recieved_packet++;
        debug2("recieved");
        process_packet(start, req, reciever);
        //threads.push_back(t1);   
    }

    cout << "PckDropRate = " << RANDOM_DROP_RATE;
    cout << "; Packet Ack Ratio: "  << (double)ack_sent/(double)recieved_packet << endl;

    return 0;
}