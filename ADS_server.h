#ifndef ADS_server
#define ADS_server

#include <bits/stdc++.h>

using namespace std;

class ADS {
  unordered_map<string, string> ip;
  public:
    ADS(vector<pair<string, string>>);
    string handle(char *req);
    void server(int);
};

#endif