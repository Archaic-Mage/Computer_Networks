#ifndef Root_server
#define Root_server

#include <bits/stdc++.h>

using namespace std;


class RDS {
  // maps .com/.edu -> IP of TLD
   unordered_map<string, string> tld_ip;
  public:
    RDS(string, string, int);
     string handle(char *req);
     void server(int);
     void log(int, string, string);
};

#endif