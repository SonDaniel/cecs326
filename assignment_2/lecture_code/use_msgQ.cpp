/*
Daniel Son
CECS 326 - Shui Lam
Assignment 2 - use_msgQ

Lecture Code

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

// declare my global message buffer
struct buf {
    long mtype; //required
    char greeting[50]; //message content
};
/*
    child_proc_one is a function that a process calls that gives it the queue id. 
    The function creates a structure for the message. It then checks for any messages
    with message type of 113. Once message is received, it displays. It then takes the message
    and adds to it 'and Adios'. It then sends the message with a message type of 114.
*/
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

/*
    child_proc_two is a function a process calls that passes in a queue id. It then 
    creates a msg object for sending and receiving. It creates a message of message type 12.
    Sends the message to the queue. It then prepares another message saying 'Hello There' and 
    sends it with a type of 113. It checks for any message of type 114 and once recieved, it displays.

*/
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

/*
    Main function is the beginning of the program. The program will create a queue id 
    from the interprocess communication. It then creates a child process and calls the
    function child_proc_one. Creates another child process and calls child_proc_two. 
    Program then waits until child process is done. After, it removes queue from kernel
    and exits.
*/
int main() {
    int qid = msgget(IPC_PRIVATE, IPC_EXCL|IPC_CREAT|0600); //create queue

    //spawning two child processes
    pid_t cpid = fork(); //create child process
    if(cpid == 0) {
        child_proc_one(qid); //call child process function one
        exit(0); //terminate child
    }
    cpid = fork();
    if(cpid == 0) {
        child_proc_two(qid); //call child process function two
        exit(0); //terminate child
    }
    while(wait(NULL) != -1); //waiting for both children to terminate

    msgctl(qid, IPC_RMID, NULL); //remove queue from kernel

    cout << "parent process: " << getpid() << " now exiting" << endl; //display parent id

    exit(0);
}