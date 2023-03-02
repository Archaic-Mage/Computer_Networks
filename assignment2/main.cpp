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

#include "NR_server.h"
#include "Root_server.h"
#include "TLD_server.h"
#include "ADS_server.h"

using namespace std;

int PORT;
string filename;

//functions called in different threads

void _client(int, string); //defined in "client.cpp"
//calls the Name resolver sever
void nr_call(NR server, int p) {
  server.server(p);
}
//calls the root server
void rds_call(RDS server, int p) {
  server.server(p);
}
//calls the  top level DNS server
void tld_call(TLD server, int p) {
  server.server(p);
}
//calls the ads servers
void ads_call(ADS* server, int p) {
  server->server(p);
}

//the respective servers are started on various threads
//client , NR, RDS, TDS, ADS
void start_thread(unordered_map<string, string> ip_addr, pair<string, vector<pair<string, string>>> name_ads_ip[]) {

  //formats the arguments to be passed while creating server instances
  vector<pair<string, string>> tld_ads[2];
  string ext[2] = {"com", "edu"};
  for(int i = 0; i<6; i++) {
    tld_ads[i/3].push_back({name_ads_ip[i].first,ip_addr[name_ads_ip[i].first+"."+ext[i/3]]});
  }

  //creating instances of NR, RDS, TDS and ADS servers
  //total of 10 servers
  NR nr_server = NR(ip_addr["rds"]);
  RDS rds_server = RDS(ip_addr["tds_com"], ip_addr["tds_edu"], PORT+54);
  TLD tld_server_com = TLD(tld_ads[0], PORT+55);
  TLD tld_server_edu = TLD(tld_ads[1], PORT+56);
  vector<ADS*> ads_server_com(3);
  for(int i = 0; i<3; i++) ads_server_com[i] = new ADS(name_ads_ip[i].second);
  vector<ADS*> ads_server_edu(3);
  for(int j = 3; j<6; j++) ads_server_edu[j-3] = new ADS(name_ads_ip[j].second);
  
  //starting threads
  thread client(_client, PORT, ip_addr["nr"]);
  thread _nr_server(nr_call, nr_server, PORT+53);
  thread _rds_server(rds_call, rds_server, PORT+54);
  thread _tld_server_com(tld_call, tld_server_com, PORT+55);
  thread _tld_server_edu(tld_call, tld_server_edu, PORT+56);
  thread _ads_server_com[3];
  for(int i = 0; i<3; i++) {
    _ads_server_com[i] = thread(ads_call, ads_server_com[i], (PORT+55) + (i+1)*2);
  }
  thread _ads_server_edu[3];
  for(int i = 0; i<3; i++) {
    _ads_server_edu[i] = thread(ads_call, ads_server_edu[i], (PORT+56) + (i+1)*2);
  }


  //waiting for  all the processes to complete
  client.join();
  _nr_server.join();
  _rds_server.join();
  _tld_server_com.join();
  _tld_server_com.join();
  for(int i = 0; i< 3; i++) {
    _ads_server_com[i].join();
    _ads_server_edu[i].join();
  }
  return;
}

//parses the input file and call the start_thread function
void parse_input_file(string filename) {
  string line;
  //opens the file
  ifstream Myfile(filename);

  getline((istream&) Myfile, line);

  cout << line << endl;

  if(line != "BEGIN_DATA") {
    cout << "Entered Wrong Filename" << endl;
    return;
  }

  unordered_map<string, string> ip_addr;

  //parsing the servers ip address
  while(line != "List_of_ADS1") {
    getline((istream&) Myfile, line);
    stringstream ss(line);
    string name;
    getline(ss, name, ' ');
    string ip;
    getline(ss, ip, ' ');
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    ip_addr[name] = ip;
  }


  //parsing the ip address in ADS servers
  pair<string, vector<pair<string, string>>> name_ads_ip[6];

  for(int i = 0; i<6; i++) {
    for(int j = 0; j < 5; j++) {
      getline((istream&) Myfile, line);
      //debug
      cout << line << endl;
      string name;
      stringstream ss(line);
      getline(ss, name, ' ');
      string ip;
      getline(ss, ip, ' ');
      //getting ads name
      stringstream sk(name);
      string word[2] = {"", ""};
      while (!sk.eof()) {
        word[0] = word[1];
        getline(sk, word[1], '.');
      } 
      name_ads_ip[i].first = word[0];
      name_ads_ip[i].second.push_back({name, ip});
    }
    getline((istream&) Myfile, line); //this skips the line "List_Of_ADS*"
  }

  if(line != "END_DATA") { //Ensures the file formatting is correct
    cout << "Wrong File contents";
    return;
  }

  Myfile.close();
  start_thread(ip_addr, name_ads_ip);
}

int main(int argc, char **argv) {
  //ensuring the arguments to the program are correct
  if (argc != 3) {
    cout << "Wrong number of arguments" <<endl;
    return 0;
  }

  PORT = atoi(argv[1]);
  if(PORT < 1024) {
    cout << "Entered Non-privileged Port" << endl;
  }

  filename = string(argv[2]);

  parse_input_file(filename);

  return 0;
}