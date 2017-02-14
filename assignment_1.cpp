/*
Daniel Son
CECS 326 - Shui Lam
Assignment 1 - Dynamic Memory Allocation
*/
#include <iostream>
#include <stdlib.h>
#include <list>
using namespace std;

/***********************************************************************/
//   MemoryBlockTable class purpose is to manage dynamic memory space. //
/***********************************************************************/
class MemoryBlockTable {
    public:
        MemoryBlockTable();     //constructor
        ~MemoryBlockTable();    //destructor
        void setSpace(int x) {space = x;}
        int getSpace() {return space;}
        void allocateMemory(bool ** pageTable, int size);
        void deallocateMemory(bool ** pageTable, int size);

    private:
        int space;
        bool mem [128];
};

//Member function definition Constructor 
MemoryBlockTable::MemoryBlockTable() { 
    space = 118; //initialize with first 10 slots occupied
    for(int ii = 0; ii < 10; ii++) { 
        mem[ii] = true;
    }
    cout << "Memory Block Table Constructed" << endl;
}

MemoryBlockTable::~MemoryBlockTable() {
    cout << "Memory Block Table Destructed." << endl;
}

void MemoryBlockTable::allocateMemory(bool ** pageTable, int size) {
    for(int ii = 0, jj = 0; ii < 128 && jj < size; ii++) {
        if(!mem[ii]) {
            pageTable[jj++] = &mem[ii];
            mem[ii] = true; //Set memory to used
        }
    }

    for(int ii = 0; ii < 128; ii++) {
        cout << &mem[ii] << " : " << mem[ii] << endl;
    }
}

void MemoryBlockTable::deallocateMemory(bool ** pageTable, int size) {
    for(int ii = 0; ii < 128; ii++) {
        for(int jj = 0; jj < size; jj++) {
            if(pageTable[jj] == &mem[ii]) {
                mem[ii] = false; //Set memory to free
                delete pageTable[jj]; //delete pointer
            }
        }
    }
}
/***********************************************************************/
//                  End of MemoryBlockTable Class                      //
/***********************************************************************/


/*******************************************************************************/
//  ProcessControlBlock class purpose is to manage a process with a page table //
/******************************************************************************/
class ProcessControlBlock {
    public:
        ProcessControlBlock(int id, int size);  //constructor
        ~ProcessControlBlock();      //destructor
        void initializePageTable(int size); //Create PageTable function
        bool ** getPageTable();
        int getPID();
        int getSize();

    private:
        int pid;            //unique process ID
        int processSize;    //Size of required memory blocks
        bool ** pageTable; //Page table
};

//Member function definition Constructor
ProcessControlBlock::ProcessControlBlock(int id, int size) {
    pid = id;
    processSize = size;
    cout << "Process Control Block #" << pid << " Constructed." << endl;
}

ProcessControlBlock::~ProcessControlBlock() {
    for(int ii = 0; ii < processSize; ii++) {
        delete pageTable[ii]; //delete pageTable content
    }
    delete pageTable; //delete pageTable
    cout << "Process Control Block #" << pid << " Destructed." << endl;
}

int ProcessControlBlock::getPID() {
    return pid;
}

void ProcessControlBlock::initializePageTable(int size) {
    pageTable = new bool * [size];
}

bool ** ProcessControlBlock::getPageTable() {
    return pageTable;
}

int ProcessControlBlock::getSize() {
    return processSize;
}
/*******************************************************************************/
//                       End of ProcessControlBlock class                      //
/******************************************************************************/

//global variables
MemoryBlockTable mem;
list<ProcessControlBlock*> readyQueue;
int counter = 0;

int menu() {
    int choice; //variable for user choice
    cout << endl << endl; //display menu
    cout << " ________________________________________________" << endl;
    cout << "| Menu                                          |" << endl;
    cout << "|_______________________________________________|" << endl;
    cout << "| Initiate a process                       [1]  |" << endl;
    cout << "| Print all processes in ready queue       [2]  |" << endl;
    cout << "| Terminate process with specific PID      [3]  |" << endl;
    cout << "|                                               |" << endl;
    cout << "|                                               |" << endl;
    cout << "| exit                                     [4]  |" << endl;
    cout << "|_______________________________________________|" << endl;
    cout << "choice: ";
    cin >> choice; //get user input
    return choice; //return user input
}

void initiate() {
    //Select random size from 1 - 30 to allocate dynamic memory
    int size = rand() % 30 + 1;
    
    //Create PCB
    ProcessControlBlock * pcb = new ProcessControlBlock(counter, size);
    

    if(mem.getSpace() - size >= 0) { //check if there are memory space
        counter++; //increment unique id
        pcb->initializePageTable(size);
        mem.allocateMemory(pcb->getPageTable(), pcb->getSize());
        readyQueue.push_back(pcb);

        mem.setSpace(mem.getSpace() - size); //update space size

    } else { //Not enough space in memory block table
        delete pcb; //clear pointer
        cout << "Error: Insufficient free blocks are available in the Memory Block Table " << endl;
    }
}

void printReadyQueue() {
    //Go through list and display all PID
    for(list<ProcessControlBlock*>::iterator it = readyQueue.begin(); it != readyQueue.end(); ++it) {
        cout << "Ready Queue Process Block ID:" << (*it)->getPID() << endl;
    }
}

void terminateProcess() {
    int choice;
    cout << "Terminate process PID: ";
    cin >> choice;
    cout << endl;

    bool found = false;
    //Go through list
    for(list<ProcessControlBlock*>::iterator it = readyQueue.begin(); it != readyQueue.end(); ++it) {
        if((*it)->getPID() == choice) { //found pid in process
            found = true;
            mem.deallocateMemory((*it)->getPageTable(), (*it)->getSize());
        }
    }

    if(!found) {
        cout << "Error: PID = " << choice << " not found." << endl;
    }
}

int main() {
    //Set variables
    bool exit = false;
    mem.setSpace(118);

    //start of program
    while(!exit) {
        switch (menu()) {
            case 1: { //Initialize a process
                initiate();
                break;
            }
            case 2: //Print all processes in ready queue
                printReadyQueue();
                break;
            case 3: //Terminate process with a speicific PID
                terminateProcess();
                break;
            case 4:
                exit = true;
                cout << "Exitting program" << endl;
                break;
            default:
                cout << "Invalid selection" << endl;
        }
    }

    return 0;
}

