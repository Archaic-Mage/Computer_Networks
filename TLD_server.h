#ifndef TLD_server
#define TLD_server

#include <bits/stdc++.h>

using namespace std;

class TLD {
   unordered_map<string, string> ads_ip;
  public:
    TLD(vector<pair<string, string>>, int);
     string handle(char *req);
     void server(int);
};

#endif