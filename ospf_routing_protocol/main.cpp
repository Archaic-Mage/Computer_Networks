// NAME: Soham Tripathy
// Roll Number: CS20B073
// Course: CS3205 Jan. 2023 semester
// Lab number: 5
// Date of submission: 28-04-2023
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): https://www.geeksforgeeks.org/udp-server-client-implementation-c/

#include <bits/stdc++.h>
#include "./Server/Server.h"
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

mutex mtx;
int NO_OF_ROUTERS;
int NO_OF_LINKS;
chrono::time_point<chrono::system_clock> start_time;
chrono::time_point<chrono::system_clock> current_time;

//Set via command line arguments

// -h flag
int HELLO_INTERVAL = 1;    
// -a flag     
int LSA_INTERVAL = 5;
// -s flag           
int SPF_INTERVAL = 20;
// -f flag          
string INPUT_FILE = "input.txt";
// -o flag
string OUTPUT_FILE = "output.txt";
// -i flag
int ROUTER_ID;

//cost interval of the router
class CostInterval {
    public:
    int min_cost;
    int max_cost;
    CostInterval() {
        this->min_cost = INT_MIN;
        this->max_cost = INT_MAX;
    }
    CostInterval(int min_cost, int max_cost) {
        this->min_cost = min_cost;
        this->max_cost = max_cost;
    }
};

//map of node_id (of neighbours in graph) to cost interval
unordered_map<int, CostInterval> cost_interval_map;

//adjacency list with cost of the routers graph
unordered_map<int, unordered_map<int, int>> graph;
//stores the shortest distance from ROUTER_ID to all the other nodes
map<int, int> dist;


//Global pointer to server object
Server* server;

//last seq no of LSA sent for each neighbour
unordered_map<int, int> last_seq_no;

/*****************************************/
/******* INITIALIZATION FUNCTIONS ********/

//Initializes the graph with INT_MAX cost for each edge
void init_cost_interval_map() {
    // parset the inputfile to get the cost intervals
    // and store them in the cost_interval_map
    FILE* file = fopen(INPUT_FILE.c_str(), "r");
    int node1, node2, min_cost, max_cost;
    //first line of the file contains the number of routers and links
    fscanf(file, "%d %d", &NO_OF_ROUTERS, &NO_OF_LINKS);
    while(fscanf(file, "%d %d %d %d", &node1, &node2, &min_cost, &max_cost) != EOF) {
        //add the nodes to the graph with cost INT_MAX
        graph[node1][node2] = INT_MAX;
        graph[node2][node1] = INT_MAX;
        //if node1 or node2 is ROUTER_ID, then add the cost interval to the map of the other node
        if(node1 == ROUTER_ID) {
            cost_interval_map[node2] = CostInterval(min_cost, max_cost);
        }
        else if(node2 == ROUTER_ID) {
            cost_interval_map[node1] = CostInterval(min_cost, max_cost);
        }
    }
}

//Creates the server object
void init_server() {
    Server* s = new Server(10000+ROUTER_ID);
    server = s;    
}

//Initialize the Last Seq No map with -1 for each neighbour
void init_last_seq_no() {
    for(auto it = cost_interval_map.begin(); it != cost_interval_map.end(); it++) {
        last_seq_no[it->first] = -1;
    }
}

/******************************************/

//check_message type
//enum of message type
enum MessageType {
    HELLO,
    HELLO_REPLY,
    LSA,
    ERROR
};

//function to check the type of message
MessageType check_message_type(string msg) {
    //first word of the message is till ';'
    string first_word = msg.substr(0, msg.find(';'));

    //check the first word
    if(first_word == "HELLO") {
        return HELLO;
    }
    else if(first_word == "HELLO_REPLY") {
        return HELLO_REPLY;
    }
    else if(first_word == "LSA") {
        return LSA;
    }
    else {
        return ERROR;
    }
}

/*********************************************/
/********* PARSING FUNCTIONS *****************/

//HELLO
void parse_hello(string msg) {
    //HELLO<sender_id>
    //send hello_reply to the sender
    int sender_id;
    sscanf(msg.c_str(), "HELLO;%d", &sender_id);
    string IP = "127.0.0.1";
    //set the edge cost to some random value between the cost interval
    int edge_cost = rand() % (cost_interval_map[sender_id].max_cost - cost_interval_map[sender_id].min_cost + 1) + cost_interval_map[sender_id].min_cost;
    server->send("HELLO_REPLY;" + to_string(ROUTER_ID) + ";" + to_string(sender_id) + ";" + to_string(edge_cost), 10000+sender_id, IP);
}

//HELLO_REPLY
void parse_hello_reply(string msg) {
    //HELLO_REPLY;<sender_id>;<receiver_id>;<edge_cost>
    int sender_id, receiver_id, edge_cost;
    sscanf(msg.c_str(), "HELLO_REPLY;%d;%d;%d", &sender_id, &receiver_id, &edge_cost);
    //update the adjacency matrix
    graph[receiver_id][sender_id] = edge_cost;
}

//LSA
void parse_lsa(string msg) {
    //split string based on ';'
    vector<string> tokens;
    stringstream check1(msg);  
    string intermediate;
    while(getline(check1, intermediate, ';')) { 
        tokens.push_back(intermediate); 
    }
    //LSA;<srcid>;SeqNo;No_of_links;dest1;cost1;dest2;cost2;...
    int srcid, seq_no, no_of_links;
    sscanf(tokens[1].c_str(), "%d", &srcid);
    sscanf(tokens[2].c_str(), "%d", &seq_no);
    sscanf(tokens[3].c_str(), "%d", &no_of_links);
    //update the adjacency list only if the seq_no is greater than the last_seq_no
    if(seq_no > last_seq_no[srcid]) {
        //update the last_seq_no
        last_seq_no[srcid] = seq_no;
        //update the adjacency list
        for(int i = 0; i < no_of_links; i++) {
            int dest, cost;
            sscanf(tokens[4+2*i].c_str(), "%d", &dest);
            sscanf(tokens[5+2*i].c_str(), "%d", &cost);
            graph[srcid][dest] = cost;
        }

        //increases the seq_no and forwards the LSA to all the other neighbours
        seq_no++;
        string IP = "127.0.0.1";
        //update the LSA message with seq_no and send it to the neighbour other than srcid
        string new_msg = "LSA;" + to_string(srcid) + ";" + to_string(seq_no) + ";" + to_string(no_of_links);
        for(int i = 0; i < no_of_links; i++) {
            new_msg += ";" + tokens[4+2*i] + ";" + tokens[5+2*i];
        }
        for(auto it = cost_interval_map.begin(); it != cost_interval_map.end(); it++) {
            if(it->first != srcid) {
                server->send(new_msg, 10000+it->first, IP);
            }
        }
    }
}

/*********************************************/

//Recieves messages from the neighbours
void recieve() {
    string msg = server->recieve();
    //check the type of message
    MessageType msg_type = check_message_type(msg);
    
    //if the message is a hello message, then send hello_reply to the sender
    //if the message is a hello_reply message, then update the adjacency matrix
    //if the message is a LSA message, then update the adjacency matrix
    switch(msg_type) {
        case HELLO:
            parse_hello(msg);
            break;
        case HELLO_REPLY:
            parse_hello_reply(msg);
            break;
        case LSA:
            parse_lsa(msg);
            break;
        default:
            break;
    }
}

//Sends the hello message to all the neighbours
void send_hello() {
    string IP = "127.0.0.1";
    //send hello message to all the neighbours
    for(auto it = cost_interval_map.begin(); it != cost_interval_map.end(); it++) {
        server->send("HELLO;" + to_string(ROUTER_ID), 10000+it->first, IP);
    }
}

//Sends the LSA message to all the neighbours
void send_lsa() {
    string IP = "127.0.0.1";
    //send lsa message to all the neighbours
    //LSA;srcid;SeqNo;NoEnteries;destid1;cost1;destid2;cost2;...
    for(auto it = cost_interval_map.begin(); it != cost_interval_map.end(); it++) {
        string msg = "LSA;" + to_string(ROUTER_ID) + ";" + to_string(0) + ";" + to_string(graph[ROUTER_ID].size());
        //add all the entries of the adjacency list to the message
        for(auto it2 = graph[ROUTER_ID].begin(); it2 != graph[ROUTER_ID].end(); it2++) {
            msg += ";" + to_string(it2->first) + ";" + to_string(it2->second) ;
        }
        server->send(msg, 10000+it->first, IP);
    }
}


//Given the adjacency list, it returns the shortest path to all the other nodes from ROUTER_ID
//the algorithm uses dijkstra's algorithm with priority queue implementation
map<int, int> dijkstra(unordered_map<int, unordered_map<int, int>> graph) {
    current_time = chrono::high_resolution_clock::now();
    //stores the parent of each node in the shortest path
    map<int, int> parent;
    //stores the visited status of each node
    map<int, bool> visited;
    //stores the nodes in the priority queue
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    //initialise the dist, parent and visited maps
    for(auto it = graph.begin(); it != graph.end(); it++) {
        dist[it->first] = INT_MAX;
        parent[it->first] = -1;
        visited[it->first] = false;
    }

    //initialise the dist of ROUTER_ID to 0
    dist[ROUTER_ID] = 0;
    //push the ROUTER_ID to the priority queue
    pq.push(make_pair(0, ROUTER_ID));

    //while the priority queue is not empty
    while(!pq.empty()) {
        //get the top element from the priority queue
        int u = pq.top().second;
        pq.pop();
        //if the node is already visited, then continue
        if(visited[u]) {
            continue;
        }
        //mark the node as visited
        visited[u] = true;
        //for all the neighbours of the node
        for(auto it = graph[u].begin(); it != graph[u].end(); it++) {
            int v = it->first;
            int weight = it->second;
            //if the distance to the neighbour is greater than the distance to the current node + the edge weight
            //then update the distance and parent of the neighbour
            if(dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                //push the neighbour to the priority queue
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    return parent;
}

//Given the parent map, print the path from ROUTER_ID to all the other nodes and the cost of the path
// format
// |    Dest    |   Path    |    Cost    |
// |    1       |   src-r1-r2-dest |    0       |
// |    2       |   1-2     |    1       |
// |    3       |   1-2-3   |    2       |
// ...

void print_path_helper(int node, map<int, int> parent, ofstream& output_file) {
    //if the node is ROUTER_ID, then print ROUTER_ID
    if(node == ROUTER_ID) {
        output_file << ROUTER_ID ;
        return;
    }
    //recursively print the path
    print_path_helper(parent[node], parent, output_file);
    output_file << "-" <<node ;
}

// prints in the output file
void print_path(map<int, int> parent) {
    //open the output file
    ofstream output_file;
    output_file.open(OUTPUT_FILE.c_str(), ios::out | ios::app);
    output_file << "Routing Table for Node No. " << ROUTER_ID << " at Time " << chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count()/1000 << endl;
    //print the header
    output_file << "|    Dest    |   Path    |    Cost    |" << endl;
    output_file << "|---|---|---|" << endl;
    //for all the nodes
    for(auto it = parent.begin(); it != parent.end(); it++) {
        //if the node is not ROUTER_ID
        if(it->first != ROUTER_ID) {
            //print the node id
            output_file << "|    " << it->first << "       |   ";
            //if the node is not reachable
            if(dist[it->first] == INT_MAX) {
                //print unreachable
                output_file << "unreachable" << "   |    ";
            }
            else {
                //print the path
                print_path_helper(it->first, parent, output_file);
                //print the cost
                output_file << "   |   " <<dist[it->first] << "   |" << endl;
            }
        }
    }
    //close the output file
    output_file.close();
}


/*******************************************/
/*************** THREADS *******************/

//recieving thread - waits for the message from the neighbours
void recieve_thread() {
    while(true) {
        recieve();
    }
}

//Sends the hello message to all the neighbours every HELLO_INTERVAL seconds
void send_hello_thread() {
    while(true) {
        send_hello();
        sleep(HELLO_INTERVAL);
    }
}

//Sends the LSA message to all the neighbours every LSA_INTERVAL seconds
void send_lsa_thread() {
    while(true) {
        send_lsa();
        sleep(LSA_INTERVAL);
    }
}

//compute the shortest path to all the other nodes from ROUTER_ID every SPF_INTERVAL seconds
void compute_shortest_path_thread() {
    while(true) {
        //compute the shortest path
        map<int, int> parent = dijkstra(graph);
        //print the shortest path
        print_path(parent);
        sleep(SPF_INTERVAL);
    }
}

/*******************************************/

//Parse the command line arguments to the global variables
void parse_args(int argc, char** argv) {
    if(argc < 13) {
        cout << "Usage: ./ospf -i <router_id> -f <input_file> -o <out-file> -h <hello_interval> -l <lsa_interval> -s <spf_interval>" << endl;
        exit(0);
    }

    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-i") == 0) {
            ROUTER_ID = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-f") == 0) {
            INPUT_FILE = argv[i+1] + string(".txt");
        }
        else if(strcmp(argv[i], "-o") == 0) {
            OUTPUT_FILE = argv[i+1] + string("-") + to_string(ROUTER_ID) + string(".txt");
        }
        else if(strcmp(argv[i], "-h") == 0) {
            HELLO_INTERVAL = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-l") == 0) {
            LSA_INTERVAL = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-s") == 0) {
            SPF_INTERVAL = atoi(argv[i+1]);
        }
    }
}

/********************************************/
/*************** MAIN ***********************/

int main(int argc, char** argv) {
    //parse the command line arguments
    parse_args(argc, argv);
    //call the initialization functions
    init_cost_interval_map();
    init_server();
    init_last_seq_no();

    //wait for all the routers to start - 2 seconds
    sleep(2);

    //start the threads for sending hello, lsa and computing shortest path
    thread send_hello(send_hello_thread);
    thread reciever(recieve_thread);
    sleep(LSA_INTERVAL);
    thread send_lsa(send_lsa_thread);
    
    start_time = chrono::high_resolution_clock::now();
    sleep(SPF_INTERVAL);
    thread compute_shortest_path(compute_shortest_path_thread);
    //join the threads
    send_hello.join();
    send_lsa.join();
    compute_shortest_path.join();
    reciever.join();
    return 0;
}

/********************************************/