/*
Daniel Son
CECS 326 - Shui Lam
Assignment 3 - shmp1 and shmc1

Assignment Prompt:
For this assignment you need to copy the following two C++ programs 
(named shmp1.cpp and shmc1.cpp with a header file registration.h) into your Linux directory, compile them
into shmp1 and shmc1 respectively. Then run shmp1 and observe what happens. Run shmp1 atleast 5 times and
observe and report the results.

file: shmp1.cpp
*/
//Libraries to include
#include "registration.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <memory.h>

using namespace std;

//declaring global variables
CLASS myclass = { "1001", "120186", "Operating Systems", 15 }; //Creating class object
#define NCHILD	3   //defining constant variable
int	shm_init( void * ); //declaring shm_init function that takes in pointer

//declaring wait_and_wrap_up function that takes in array of int, pointer, and int
void wait_and_wrap_up( int [], void *, int );  
void rpterror( char *, char * ); //declaring rpterror function that takes in two pointer chars

main(int argc, char *argv[]) {
    int child[NCHILD], i, shmid, semid; //Initialize variables
    void	*shm_ptr;   //initialize shm ptr variable
    char	ascshmid[10], ascsemid[10], pname[14];  //Initialzing character array variables
    strcpy (pname, argv[0]);    //Set pname passed in argument
    shmid = shm_init(shm_ptr);  //call shm_init function and get back share memory id
    sprintf (ascshmid, "%d", shmid);

    //Loop through NCHILD amount of times
    for (i = 0; i < NCHILD; i++) {
        child[i] = fork(); //Create a child process and save in array

        //conditional check on child array of index i
        switch (child[i]) {
            case -1: //Case where id is -1
                rpterror ("fork failure", pname); //
                exit(1); //exit program with code 1

            case 0: 
                sprintf (pname, "shmc%d", i+1); //print pname with index number
                execl("shmc1", pname, ascshmid, (char *)0); //execute program shmc1 passing in params
                perror ("execl failed"); //print error 
                exit (2); //exit program with code 2
        }
    }

    //call function passing in array, shared memory pointer and id
    wait_and_wrap_up (child, shm_ptr, shmid);
}

int shm_init(void *shm_ptr) {
    int	shmid; //declare variable for shared memory id

    //get the id by ftok with size of CLASS and 
    shmid = shmget(ftok(".",'u'), sizeof(CLASS), 0600 | IPC_CREAT);

    //Check if shared memory cannot be obtained
    if (shmid == -1) {
        perror ("shmget failed"); //print error message
        exit(3); //Exit program with code 3
    }

    //Set shared memory pointer to attached shared memory piece with shared memory
    //id with address 0, and flag 0.
    shm_ptr = shmat(shmid, (void * ) 0, 0);

    //check to see if shared memory pointer is attached
    if (shm_ptr == (void *) -1) {
        perror ("shmat failed"); //print error message
        exit(4); //Exit program with code 4
    }
    
    //copy block of memory to shm_ptr using myclass
    memcpy (shm_ptr, (void *) &myclass, sizeof(CLASS) );
    
    return (shmid); //Return shared memory id

}

void wait_and_wrap_up(int child[], void *shm_ptr, int shmid) {
    int wait_rtn, w, ch_active = NCHILD; //declare variables

        //loop through while ch_active is greater than 0
        while (ch_active > 0) {
        wait_rtn = wait( (int *)0 ); //assign wat_rtn the wait sign

        for (w = 0; w < NCHILD; w++) //Loop through chidlren
            if (child[w] == wait_rtn) { //If children are done
                ch_active--; //decrement childs active 
                break; //break out
            }
        }
    cout << "Parent removing shm" << endl; //print out message
    shmdt (shm_ptr); //detach shared memory poitner
    shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0); //clear shared memory 
    exit (0); //exit program with code 0
}

void rpterror(char *string, char *pname) {
    char errline[50]; //declare variable 
    sprintf (errline, "%s %s", string, pname); //format message
    perror (errline); //print error message
}

