/*
Daniel Son
CECS 326 - Shui Lam
Assignment 2 - Process Communication

Sender Program Code

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
    Program will then ask for user input for a message to send. Once user 
    inputs a message, message will be sent to queue with a message type of 113.
*/
int main(int argc, char* argv[]) {
    int qid = msgget(ftok(".",'u'), 0);

    struct buf {
        long mtype; //required
        char greeting[50]; //message content
    };

    buf msg; //creates message object
    int size = sizeof(msg) - sizeof(long); //creates size of message object
    char message[50];
    cout << "(Sender: " << getpid() << ") " << "message to send: "; 
    cin.getline(message,sizeof(message)); //get user input for message
    cout << endl; //spacing of displaying message

    //prepare my message to send
    strcpy(msg.greeting, message); //adds text to struct greeting

    cout << "(Sender: "<< getpid() << ") : sent message." << endl << endl; //displays process id
    msg.mtype = 113; //sets message type to 113
    msgsnd(qid, (struct msgbuf*)&msg, size, 0); //sends message of type 113 to IPC

    exit(0); //exits program
}