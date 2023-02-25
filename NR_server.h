#ifndef NR_server
#define NR_server

#include <bits/stdc++.h>

using namespace std;

class NR {
  string root;
  string name_requested;
  NR(string);
  pair<string, int> extract(string);
  string request(string, int, int);
  void server(int);
};

#endif