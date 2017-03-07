#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;

void receiveMessage(int qid) {
    struct buf {
        long mtype; //required
        char greeting[50]; //message content
    };

    buf msg; //creates message object
    int size = sizeof(msg) - sizeof(long); //creates size of message object

    msgrcv(qid, (struct msgbuf*) &msg, size, 113, 0); //receive message with type 113
    cout << getpid() << " (Reciever): Received message" << endl; //display pid getting message
    cout << "message output: " << msg.greeting << endl << endl; //display message
}