#include <bits/stdc++.h>
#include <thread>

using namespace std;

void foo(int a) {
    int b = a*2;
    cout << b;
    return;
}

int main() {
    thread _foo(foo, 5);
    _foo.join();
    cout << "done";
    return 0;
}