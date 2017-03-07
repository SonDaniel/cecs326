#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;

void child_proc_one() {
    //create my msgQ with key value from ftok()
    int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

    //declare my message buffer
    struct buf {
        long mtype; //required
        char greeting[50]; //mesg content
    };

    buf msg;
    int size = sizeof(msg) - sizeof(long);

    msgrcv(qid, (struct msgbuf*)&msg, size, 113, 0);
    
    cout << getpid() << ":gets message " << endl;
    cout << "message: " << msg.greeting << endl;

    strcat(msg.greeting, " and Adios.");
    cout << getpid() << ": sends reply" << endl;
    msg.mtype = 114; //only reading mesg with type mtype = 114;
    msgsnd(qid, (struct msgbuf*)&msg, size, 0);
    cout << getpid() << ": now exits" << endl;
}

void child_proc_two() {
    //pause child proc two
    sleep(3);

    int qid = msgget(ftok(".",'u'), 0);

    //declare my message buffer
    struct buf {
        long mtype;
        char greeting[50];
    };
    buf msg;
    int size = sizeof(msg) - sizeof(long);

    //sending a message that will never be read
    strcpy(msg.greeting, "Fake message");
    msgsnd(qid, (struct msgbuf*)&msg, size, 0);

    //prepare my message to send
    strcpy(msg.greeting, "Hello There");

    cout << getpid() << ":sends greeting" << endl;
    msg.mtype = 113;
    msgsnd(qid, (struct msgbuf*)&msg, size, 0);

    msgrcv(qid, (struct msgbuf*)&msg, size, 114, 0);
    cout << getpid() << ":gets reply" << endl;
    cout << "reply: " << msg.greeting << endl;
    cout << getpid() << ":now exits" << endl;
}

int main() {
    //spawning two child processes
    pid_t cpid = fork();
    if(cpid == 0) {
        child_proc_one();
        exit(0);
    }
    cpid = fork();
    if(cpid == 0) {
        child_proc_two();
        exit(0);
    }

    while(wait(NULL) != -1); //waiting for both children to terminate

    int qid = msgget(ftok(".", 'u'), 0);
    msgctl(qid, IPC_RMID, NULL);

    cout << "parent proc: " << getpid() << " now exits" << endl;

    exit(0);
    
}