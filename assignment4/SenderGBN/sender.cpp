// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 4
// Date of submission: 04-03-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://www.geeksforgeeks.org/udp-server-client-implementation-c/


#include <bits/stdc++.h>
#include <random>
#include "headers/client.h"
#include "headers/packet.h"
#include "headers/debug_print.h"
#include "headers/tbuffer.h"
#include <mutex>

using namespace std;

#define _time_t chrono::_V2::system_clock::time_point

//packet information
int PACKET_LENGTH;
int PACKET_GEN_RATE;
int MAX_BUFFER_SIZE;
int MAX_PACKETS;
string IP;
int PORT;
_time_t gen_start;
int transmissions = 0;
bool STOP = false;
bool sent = false;

//curr sequence number for the packets to go in the buffer
char SEQ_NO;

//RTT avg computations
int RTT_avg = 0;
//packets acknowledged
int packets_ack = 0;

int WINDOW_SIZE;
int default_timeout = 100000; // 100miliseconds


//creating buffer for the sender
T_Buffer tranmission_buffer = T_Buffer();
//mutex lock for the buffer
mutex buffer_t;

void debug2(auto x) {
    return;
    cout << x << endl;
}


void re_eval_RTT(int RTT)
{
    RTT_avg = ((RTT_avg * (packets_ack)) + RTT) / (packets_ack+1);
}

// Packet generation
void _gen_packet()
{
    while (1)
    {
        srand(time(0));
        clock_t start = clock();
        PACKET p = PACKET();
        _time_t gen = chrono::high_resolution_clock::now();
        chrono::microseconds time_gen = chrono::duration_cast<chrono::microseconds>(gen - gen_start);
        string msg = "";
        for (int i = 0; i < PACKET_LENGTH - 1; i++)
        {
            msg += (rand() % 128);
        }
        p.message = msg;
        // buffer_t.lock();
        if (tranmission_buffer.is_free())
        {
            debug2("packet added to buffer");
            debug2(int(SEQ_NO));
            p.SEQ = SEQ_NO;
            SEQ_NO++;
            if (SEQ_NO < 0)
                SEQ_NO += 128;
            tranmission_buffer.add(p);
            tranmission_buffer.time_generated.push_back(time_gen);
        }
        // buffer_t.unlock();
        clock_t end = clock();
        double duration = (double)(end - start) / CLOCKS_PER_SEC;
        int microseconds = duration * 1000000;
        this_thread::sleep_for(chrono::microseconds(1000000 / PACKET_GEN_RATE - microseconds));
        if(packets_ack >= MAX_PACKETS || STOP) {
            debug2("exit gen");
            return;
        }
    }
}

vector<pair<int, _time_t>> index_timeouts(128);
vector<int> attempts(128, 0);

int to_send = 0;
Client* sender;

void init_timeouts() {
    for(int i = 0; i<128; i++) {
        index_timeouts[i].first = -1;
    }
}

bool check_timeouts() {
    _time_t end = chrono::high_resolution_clock::now();
    for(int i = 0; i<128; i++) {
        if(index_timeouts[i].first != -1) {
            _time_t start = index_timeouts[i].second;
            chrono::microseconds duration = chrono::duration_cast<chrono::microseconds>(end-start);
            if(packets_ack <= 10 && duration.count() > default_timeout) {
                index_timeouts[i].first = -1;
                return true;
            } else if(packets_ack > 10 && duration.count() > 2*RTT_avg) {
                index_timeouts[i].first = -1;
                return true;
            }
        }
    }
    return false;
}

void send_packet()
{
    init_timeouts();
    while (1)
    {

        if(packets_ack >= MAX_PACKETS) return;
        buffer_t.lock();
        if(check_timeouts()) {
            buffer_t.unlock();
            debug2("time_out");
            to_send = 0;
            continue;
        }
        // buffer_t.lock();
        if (to_send < WINDOW_SIZE && to_send < tranmission_buffer.buffer.size())
        {
            sender->send(tranmission_buffer.buffer[to_send].stringify());
            sent = true;
            debug2("send data");
            debug2(int(tranmission_buffer.buffer[to_send].SEQ));
            index_timeouts[tranmission_buffer.buffer[to_send].SEQ].first = to_send;
            index_timeouts[tranmission_buffer.buffer[to_send].SEQ].second = chrono::high_resolution_clock::now(); 
            attempts[tranmission_buffer.buffer[to_send].SEQ]++;
            if(attempts[tranmission_buffer.buffer[to_send].SEQ]-1 > 5) {
                STOP = true; 
                buffer_t.unlock();
                debug2("exit sender");
                return;
            }
            transmissions++;
            to_send++;
        }
        buffer_t.unlock();
    }
}
void recieve_packet()
{
    while (1)
    {
        debug2("reciever thread running");
        buffer_t.lock();
        if(!sent) {
            buffer_t.unlock();
            continue;
        }
        PACKET res = unpack(sender->recieve());
        debug2("recieved");
        sent = false;
        debug2("recieved ack packet");
        debug2(int(res.SEQ));

        _time_t end = chrono::high_resolution_clock::now();

        int ack_till = index_timeouts[res.SEQ].first;

        for(int i = 0; i<=ack_till; i++) {
            index_timeouts[tranmission_buffer.buffer[0].SEQ].first = -1;
            _time_t start = index_timeouts[tranmission_buffer.buffer[0].SEQ].second;
            chrono::microseconds duration = chrono::duration_cast<chrono::microseconds>(end-start);
            debug2(duration.count());
            debug2("microseconds");
            //information display
            debug("Seq ");
            debug(int(tranmission_buffer.buffer[0].SEQ));
            debug(": ");
            debug("Time Generated: ");
            chrono::microseconds time_gen = tranmission_buffer.time_generated[0];
            debug(time_gen.count()/1000);
            debug(":");
            debug(time_gen.count()%1000);
            debug(" RTT: ");
            debug(duration.count());
            debug(" Number of Attempts: ");
            debug(attempts[tranmission_buffer.buffer[0].SEQ]);
            debug("\n");

            attempts[tranmission_buffer.buffer[0].SEQ] = 0;
            re_eval_RTT(duration.count());
            if(STOP) {
                debug2("exit reci");
                return;
            }
            tranmission_buffer.buffer.erase(tranmission_buffer.buffer.begin());
            tranmission_buffer.time_generated.erase(tranmission_buffer.time_generated.begin());
            packets_ack++;
            to_send--;
        }

        //changing indexes after acknowledgement
        for(int i = 0; i<128; i++) {
            if(index_timeouts[i].first != -1) {
                index_timeouts[i].first -= (ack_till + 1);
            }
        }

        buffer_t.unlock();
        if(packets_ack >= MAX_PACKETS) return;
    }
}

void handle_args(int argc, char** argv) {
    if(argc < 15) {
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
        } else if(option == "-l") {
            int value = atoi(argv[i]);
            PACKET_LENGTH = value;
        } else if(option == "-s") {
            IP = argv[i];
        } else if(option == "-r") {
            int value = atoi(argv[i]);
            PACKET_GEN_RATE = value;
        } else if(option == "-w") {
            int value = atoi(argv[i]);
            WINDOW_SIZE = value;
        } else if(option == "-f") {
            int value = atoi(argv[i]);
            MAX_BUFFER_SIZE = value;
            tranmission_buffer.max_size = MAX_BUFFER_SIZE;
        }
    }

    debug2(IP);
    debug2(PORT);
    debug2(PACKET_LENGTH);
    debug2(PACKET_GEN_RATE);
    debug2(MAX_PACKETS);
    debug2(WINDOW_SIZE);
    debug2(MAX_BUFFER_SIZE);
}

int main(int argc, char **argv)
{

    handle_args(argc, argv);

    sender = new Client(IP, PORT);

    gen_start = chrono::high_resolution_clock::now();
    thread generate_packets(_gen_packet);
    thread _sender(send_packet);
    thread _recieving(recieve_packet);
    _sender.join();
    _recieving.detach();
    generate_packets.join();

    if(STOP) {
        cout << "Incomplete transmission of packets due to excess retransmission" << endl;
        return 0;
    }
    cout << "PckRate = ";
    cout << PACKET_GEN_RATE;
    cout << "; PckLength = ";
    cout << PACKET_LENGTH;
    cout << "; Retrans. Ratio = ";
    cout << (double)transmissions/(double)packets_ack;
    cout << "; AvgRTT = ";
    cout << RTT_avg/1000 << ":" << RTT_avg%1000;
    cout << "\n";

    return 0;
}