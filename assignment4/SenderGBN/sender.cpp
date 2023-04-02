#include <bits/stdc++.h>
#include "headers/client.h"
#include "headers/packet.h"

using namespace std;

// void handle_args(int argc, char** argv) {
//     if(argc < 7) {
//         cout << "Wrong number of arguments" << endl;
//         exit(1);
//     }

//     for(int i = 1; i<argc; i++) {
//         string option = string(argv[i]);
//         if(option == "-d") {
//             DEBUG = true;
//             continue;
//         }
//         i++;
//         int value = atoi(argv[i]);
//         if(option == "-p") {
//             PORT = value;
//         } else if(option == "-n") {
//             MAX_PACKETS = value;
//         } else if(option == "-e") {
//             RANDOM_DROP_RATE = value;
//         }
//     }
// }

int main(int argc, char** argv) {

//    handle_args(argc, argv);

    Client sender = Client("127.0.0.1", 8080);
    sender.send("something");
    cout << "sent" << endl;
}