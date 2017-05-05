/*
Daniel Son
CECS 326 - Shui Lam
Assignment 4 - shmp1 and shmc1 revised with Semaphore

Assignment Prompt:
As you have come to understand, the shmp1.cpp and shmc1.cpp you ran in Assignment 3 have serious deficiency due to race condition. In this assignment you are to correct the problem using the semaphore mechanisms that Linux provides.

Two implementations of semaphore are commonly available on most distributions of UNIX and Linux operating systems. The System V implementation includes semget(), semctl(), and semop(), and a struct sembuf used in the semop() calls, which are defined in <sys/sem.h>. The POSIX implementation includes sem_destroy(), sem_wait(), sem_post(), sem_open() for named semaphore and sem_init() for unnamed semaphore, which are defined in <semaphore.h>. You may use either one of these implementations. Details on the definition of these functions and their use may be found on Linux main pages.

file: shmc1.cpp
compile: g++ shmc1.cpp -o shmc1 -lpthread -lrt
*/
//Libraries to include
#include "registration.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <memory.h>
#include <sys/sem.h>
#include <semaphore.h>

using namespace std;

//declaring global variables
CLASS *class_ptr; // pointer of CLASS variable
void *memptr; //memory pointer variable
char *pname; //pname pointer variable
int	shmid, ret; //shared memory id and ret variable

sem_t sem_des; // create semaphore data type

void rpterror(char *), srand(), perror(), sleep(); //declare function rpterror
void sell_seats(); //declare sell_seats function

main(int argc, char* argv[]) {
    if (argc < 2) { //Check to see if argument pass out is less than 2
        fprintf (stderr, "Usage:, %s shmid\n", argv[0]); //print out message
        exit(1); //exit program with error code 1
    }

    pname = argv[0]; //assign pname first argument
    sscanf (argv[1], "%d", &shmid); //read the formatted data of second arg value to shmid
    
    //Set memory pointer to attach to shared memory piece of shmid with address of 0 and flag 0
    memptr = shmat (shmid, (void *)0, 0);
    sem_init(&sem_des,1, 1); // initialize unnamed semaphore using sem_des. value init to 1
    if (memptr == (char *)-1 ) { //check o see if memory pointer was set
        rpterror ("shmat failed"); //print error
        exit(2); //exit program with error code 2
    }
    class_ptr = (struct CLASS *)memptr; //assign class_ptr the struct class of memptr
    sell_seats(); //calling function sell_seats
    ret = shmdt(memptr); //detach shared memory piece of memory pointer and assign to ret
    exit(0); //Exit program with error code 0
}

void sell_seats() {
    int all_out = 0; //declare variable with value 0
    srand((unsigned) getpid()); //get random process id
    while(!all_out) {
        sem_wait(&sem_des); // wait until the semaphore is released so others can enter critical section
        if (class_ptr->seats_left > 0) {
            sleep ( (unsigned)rand()%5 + 1); //wait from a random 1 - 5 amount of time
            class_ptr->seats_left--; //decrement seats_left in class_ptr
            // sleep ( (unsigned)rand()%5 + 1); //wait from a random 1 - 5 amount of time
            cout << pname << " SOLD SEAT -- " << class_ptr->seats_left << " left" << endl; //display message
            sem_post(&sem_des); // critical section is done. release semaphore
        } else {
            sem_post(&sem_des); // critical section is done. release semaphore.
            all_out++; //increment all_out to get out of while loop
            cout << pname << " sees no seats left" << endl; //display message 
        }
        sleep ( (unsigned)rand()%10 + 1); //sleep process for a random 1 - 10 amount of time
    }
}

void rpterror(char* string) {
    char errline[50]; //declare variable that holds error message
    sprintf (errline, "%s %s", string, pname); //display message
    perror (errline); //print out message
}