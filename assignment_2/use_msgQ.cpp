#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;

// declare my global message buffer
struct buf {
    long mtype; //required
    char greeting[50]; //message content
};

void child_proc_one(int qid) {
    buf msg; //creates message object
    int size = sizeof(msg) - sizeof(long); //creates size of message object

    msgrcv(qid, (struct msgbuf*) &msg, size, 113, 0); //receive message with type 113

    cout << getpid() << ": gets message" << endl; //display pid getting message
    cout << "message :" << msg.greeting << endl; //display message

    strcat(msg.greeting, " and Adios."); //concat greeting with 'and adios'
    cout << getpid() << ": sends reply" << endl; //displays pid
    msg.mtype = 114; //prepare message with type mtype = 114
    msgsnd(qid, (struct msgbuf *)&msg, size, 0); //sends message to IPC
    cout << getpid() << ": now exits" << endl; //exit text
}

void child_proc_two(int qid) {
    buf msg; //creates message object
    int size = sizeof(msg) - sizeof(long); //creates size of message object

    //sending a message that will never be read
    msg.mtype = 12; //sets message type to 12
    strcpy(msg.greeting, "Fake message"); //adds text to struct greeting
    msgsnd(qid, (struct msgbuf*)&msg, size, 0); //sends message to IPC

    //prepare my message to send
    strcpy(msg.greeting, "Hello There"); //adds text to struct greeting

    cout << getpid() << ": sends greeting" << endl; //displays process id
    msg.mtype = 113; //sets message type to 113
    msgsnd(qid, (struct msgbuf*)&msg, size, 0); //sends message of type 113 to IPC

    msgrcv(qid, (struct msgbuf*)&msg, size, 114, 0); //receive message from IPC of type 114
    cout << getpid() << ": gets reply" << endl; //display pid getting reply
    cout << "reply: " << msg.greeting << endl; //display message
    cout << getpid() << ": now exits" << endl; //display exit of pid 
}

int main() {
    int qid = msgget(IPC_PRIVATE, IPC_EXCL|IPC_CREAT|0600);

    //spawning two child processes
    pid_t cpid = fork();
    if(cpid == 0) {
        child_proc_one(qid);
        exit(0);
    }
    cpid = fork();
    if(cpid == 0) {
        child_proc_two(qid);
        exit(0);
    }
    while(wait(NULL) != -1); //waiting for both children to terminate

    msgctl(qid, IPC_RMID, NULL); //remove queue from kernel

    cout << "parent proc: " << getpid() << " now exits" << endl; 

    exit(0);
}