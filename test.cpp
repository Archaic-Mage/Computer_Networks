#include <bits/stdc++.h>
using namespace std;

int main(){

    char *a = (char*) malloc(6);

    sprintf(a, "hello");
    
    string k = string(a);

    cout << k << endl;
    cout << a << endl;


    return 0;
}