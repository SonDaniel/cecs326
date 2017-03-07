#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "sender.cpp"
#include "receiver.cpp"
using namespace std;

int main(){
    int qid = msgget(IPC_PRIVATE, IPC_EXCL|IPC_CREAT|0600); //create queue

    //spawning two child processes
    pid_t cpid = fork(); //create child process
    if(cpid == 0) {
        sendMessage(qid); //call child process function one
        exit(0); //terminate child
    }
    cpid = fork();
    if(cpid == 0) {
        receiveMessage(qid); //call child process function two
        exit(0); //terminate child
    }
    while(wait(NULL) != -1); //waiting for both children to terminate

    msgctl(qid, IPC_RMID, NULL); //remove queue from kernel

    cout << "parent proc: " << getpid() << " now exits" << endl; //display parent id

    exit(0);
}