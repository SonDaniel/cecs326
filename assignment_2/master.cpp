/*
Daniel Son
CECS 326 - Shui Lam
Assignment 2 - Process Communication

Assignment Prompt:
For this assignment you need to write three C++ programs named master.cpp, sender.cpp, and receiver.cpp, which should be compiled into master, sender, and receiver, respectively.  Together they should do the following:

The master process should first acquire a message queue from the operating system, followed by creating two child processes, with one to execute sender and the other to execute receiver.  Identify sender and receiver by outputting their process IDs (PIDs).  master then waits for both child processes to return.

The sender process should prompt user for a line of input, then send the input line to receiver via the message queue created by master.  Make sure that the prompt identifies the process making the prompt is sender along with its PID.

The receiver process retrieves a message from the message queue, and output the message on the screen.  Make sure the output identifies that the line is printed by receiver along with PID.
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
    Main function creates a queue id. Fork's two child process.
    First child process will execute sender. Second child process will
    execute receiver. Main function will wait until both child are done.
    Once done, main function will remove queue from kernel and exit.
*/
int main(){
    //create queue id
    int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

    //spawning two child processes
    pid_t cpid = fork(); //create child process
    if(cpid == 0) {
        cout << "Child process (Sender) ID: " << cpid << endl;
        execl("./sender",0);
        exit(0); //terminate child
    }
    cpid = fork();
    if(cpid == 0) { //create another child process
        cout << "Child process (Reciever) ID: " << cpid << endl;
        execl("./receiver",0);
        exit(0); //terminate child
    }
    while(wait(NULL) != -1); //waiting for both children to terminate
    msgctl(qid, IPC_RMID, NULL); //remove queue from kernel
    cout << "parent process ID " << getpid() << ": exit" << endl; //display parent id
    exit(0); //terminates parent
}