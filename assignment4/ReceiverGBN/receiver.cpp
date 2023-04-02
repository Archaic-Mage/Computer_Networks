#include <thread>
#include <random>
#include <chrono>
#include <bits/stdc++.h>

#include "./headers/Server.h"
#include "./headers/debug_print.h"
#include "./headers/packet.h"

using namespace std;

int RANDOM_DROP_RATE = 1;
int PORT = 0;
int MAX_PACKETS = 0;
char NES = 0;

vector<thread*> threads;

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

    //debug
    cout << "done handling" << endl;
}

void process_packet(clock_t start, string req, Server reciever) {
    string drop = "false";
    bool dropped = will_drop();
    if(dropped) drop = "true";

    //timing details
    clock_t now = clock();
    double elapsed = (double)(now - start) / CLOCKS_PER_SEC;
    int milliseconds = int(elapsed*1000);
    int microseconds = int((elapsed*1000 - milliseconds)*1000);

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


int main(int argc, char** argv) {

    handle_args(argc, argv);

    Server reciever = Server(PORT);
    clock_t start = clock();

    while(MAX_PACKETS) {
        //debug
        cout << "Started while loop" << endl;
        string req = reciever.recieve();
        thread t1 = thread(process_packet, start, req, reciever);
        //threads.push_back(t1);   
    }

    return 0;
}