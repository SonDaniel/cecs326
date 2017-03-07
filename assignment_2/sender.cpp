#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;

void sendMessage(int qid) {
    struct buf {
        long mtype; //required
        char greeting[50]; //message content
    };

    buf msg; //creates message object
    int size = sizeof(msg) - sizeof(long); //creates size of message object
    char message[50];
    cout << "message to send: ";
    cin.getline(message,sizeof(message));
    cout << endl;

    //prepare my message to send
    strcpy(msg.greeting, message); //adds text to struct greeting

    cout << getpid() << " (Sender) : sent message." << endl << endl; //displays process id
    msg.mtype = 113; //sets message type to 113
    msgsnd(qid, (struct msgbuf*)&msg, size, 0); //sends message of type 113 to IPC
}