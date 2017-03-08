/*
Daniel Son
CECS 326 - Shui Lam
Assignment 2 - Process Communication

Receiver Program code

*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;
/*
    Main function will grab queue id and create a struct for the message.
    Program will then grab message with message type 113. Once grabbed, it will
    display to the user.
*/
int main(int argc, char* argv[]) {
    int qid = msgget(ftok(".",'u'), 0);
    struct buf {
        long mtype; //required
        char greeting[50]; //message content
    };

    buf msg; //creates message object
    int size = sizeof(msg) - sizeof(long); //creates size of message object

    msgrcv(qid, (struct msgbuf*) &msg, size, 113, 0); //receive message with type 113
    cout <<  "(Reciever: "  << getpid() << "): Received message." << endl; //display pid getting message
    cout <<  "(Reciever: "  << getpid() << "): Message Received: " << msg.greeting << endl << endl; //display message
    exit(0);
}