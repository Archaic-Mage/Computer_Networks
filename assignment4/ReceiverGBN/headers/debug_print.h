#ifndef HDEBUG
#define HDEBUG

#include <iostream>
#include <vector>
using namespace std;

bool DEBUG = false;

//prints the value if DEBUG is set to true
void debug(auto x) {
    if(!DEBUG) return;
    cout << x;
}


#endif