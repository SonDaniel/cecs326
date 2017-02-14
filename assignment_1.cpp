/*
Daniel Son
CECS 326 - Shui Lam
Assignment 1 - Dynamic Memory Allocation
*/
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <new>
using namespace std;

/***********************************************************************/
//   MemoryBlockTable class purpose is to manage dynamic memory space. //
/***********************************************************************/
/*
    MemoryBlockTable()   : Constructor function that will initialize a space 
                           variable to 118 and set mem array first 10 index 
                           to true.

    ~MemoryBlockTable()  : Destructor function that will delete MemoryBlockTable

    void setSpace(int x) : Function will allow user to set variable space

    int getSpace()       : Function will grab and return int space variable

    void allocateMemory(int * pageTable, int size ) : 
                           Function will take pageTable pointer and allocate index of memory
                           to the int array and set mem array from false to true. Parameter 
                           size is the size of the pageTable.

    void deallocateMemory(int * pageTable, int size):
                           Function will take pageTable pointer and deallocate mem array using 
                           int array pageTable stored. Parameter size is the size of the pageTable.
*/
class MemoryBlockTable {
    public:
        MemoryBlockTable();     //constructor
        ~MemoryBlockTable();    //destructor
        void setSpace(int x) {space = x;}
        int getSpace() {return space;}
        void allocateMemory(int * pageTable, int size);
        void deallocateMemory(int * pageTable, int size);

    private:
        int space;
        bool mem [128];
};

//Member function definition Constructor 
MemoryBlockTable::MemoryBlockTable() { 
    space = 118; //initialize with first 10 slots occupied
    for(int ii = 0; ii < 10; ++ii) { 
        mem[ii] = true;
    }
    cout << "Memory Block Table Constructed" << endl;
}

//Member function definition Destructor
MemoryBlockTable::~MemoryBlockTable() {
    cout << "Memory Block Table Destructed." << endl;
}

//Member function definition allocationMemory
void MemoryBlockTable::allocateMemory(int * pageTable, int size) {
    for(int ii = 0, jj = 0; ii < 128 && jj < size; ++ii) {
        if(!mem[ii]) {
            pageTable[jj] = ii; //initialize memory location
            jj++; //increment counter
            mem[ii] = true; //Set memory to used
        }
    }
}

//Member function definition deallocateMemory
void MemoryBlockTable::deallocateMemory(int * pageTable, int size) {
    for(int ii = 0; ii < size; ++ii) {
        mem[pageTable[ii]] = false; //Set memory to free
    }
    cout << space << endl;
    space = space + size; //update space variable
    cout << space << endl;
}
/***********************************************************************/
//                  End of MemoryBlockTable Class                      //
/***********************************************************************/


/*******************************************************************************/
//  ProcessControlBlock class purpose is to manage a process with a page table //
/******************************************************************************/
/*
    ProcessControlBlock(int id, int size) : Constructor function which takes in Parameters 
                                            id which is a unique id for PID. Size for the 
                                            memory size the PCB requires.

    ~ProcessControlBlock()                : Destructor function which will delete PageTable
                                            pointer to retrieve back memory.

    void initializePageTable(int size)    : Function will take in parameter of size which is
                                            used to dynamically initialize the pageTable size.

    int * getPageTable()                  : Function that returns the pointer of the pageTable.

    int getPID()                          : Function will return a integer of the PID.

    int getSize()                         : Function will return a integer of the size of the process.
*/
class ProcessControlBlock {
    public:
        ProcessControlBlock(int id, int size);  //constructor
        ~ProcessControlBlock();      //destructor
        void initializePageTable(int size); //Create PageTable function
        int * getPageTable();
        int getPID();
        int getSize();

    private:
        int pid;            //unique process ID
        int processSize;    //Size of required memory blocks
        int * pageTable; //Page table
};

//Member function definition Constructor
ProcessControlBlock::ProcessControlBlock(int id, int size) {
    pid = id;
    processSize = size;
    pageTable = NULL;
    cout << "Process Control Block #" << pid << " Constructed." << endl;
}

//Member function definition Destructor
ProcessControlBlock::~ProcessControlBlock() {
    delete [] pageTable; //delete pageTable
    cout << "Process Control Block #" << pid << " Destructed." << endl;
}

//returns PID
int ProcessControlBlock::getPID() {
    return pid;
}

//Initializes Page Table in Process Control Block
void ProcessControlBlock::initializePageTable(int size) {
    pageTable = new int[size];
}

//Returns Page Table stored in Process Control Block
int * ProcessControlBlock::getPageTable() {
    return pageTable;
}

//Returns the size of the memory of the Process Control Block
int ProcessControlBlock::getSize() {
    return processSize;
}
/*******************************************************************************/
//                       End of ProcessControlBlock class                      //
/******************************************************************************/

//global variables
MemoryBlockTable mem;
vector<ProcessControlBlock*> readyQueue;
int counter = 0; //Counter for unique id

/*
Menu function will display a menu and grab user input. Returns an integer of the 
user input
*/
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

//Initialization function that will create a PCB and allocate Memory to MBT
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
        cout << "Memory Space: " << mem.getSpace() << endl;
    } else { //Not enough space in memory block table
        delete pcb; //delete PCB pointer
        cout << "Error: Insufficient free blocks are available in the Memory Block Table " << endl;
    }
}

//Prints out the PID of the Ready Queue
void printReadyQueue() {
    //Go through list and display all PID
    for(int ii = 0; ii < readyQueue.size(); ii++) {
        ProcessControlBlock * temp = readyQueue.at(ii);
        cout << "Ready Queue Process Block ID: " << (temp)->getPID() << " |  Size: " << (temp)->getSize() << " Blocks" << endl;
    }
}

//User selects a Process to terminate based of PID
void terminateProcess() {
    int choice;
    cout << "Terminate process PID: ";
    cin >> choice;
    cout << endl;

    bool found = false;
    //Go through list
    for(int ii = 0; ii < readyQueue.size(); ii++) {
        ProcessControlBlock * temp = &(*readyQueue[ii]); //get PCB
        if((temp)->getPID() == choice) { //found pid in process
            found = true; //boolean found
            mem.deallocateMemory((temp)->getPageTable(), (temp)->getSize()); //take back memory
            delete temp;
            readyQueue.erase(readyQueue.begin() + ii); //Remove PCB from Ready Queue
        }
    }

    if(!found) { //If not found then display error
        cout << "Error: PID = " << choice << " not found." << endl;
    }
}

//Function will check ReadyQueue and delete any instance of any PCB inside readyQueue
void exitProgram() {
    for(int ii = 0; ii < readyQueue.size(); ii++) {
        ProcessControlBlock * temp = &(*readyQueue[ii]); //create temporary variable to get PCB
        delete temp; //Delete PCB
    }
}

//Main start of function
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
            case 4: //Exit program
                exit = true;
                exitProgram();
                cout << "Exitting program" << endl;
                break;
            default: //Catches any invalid inputs
                cout << "Invalid selection" << endl;
        }
    }

    
    return 0;
}