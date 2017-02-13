#include <iostream>
#include <stdlib.h>
#include <list>
using namespace std;

class MemoryBlockTable {
    public:
        void setSpace(int x) {space = x;}
        int getSpace() {return space;}
    private:
        int space;
        bool mem [128];
};

class ProcessControlBlock {
    public:

    private:
        int pid;
        
};

//global variables
MemoryBlockTable mem;
int pid = 0;

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
    struct pcb {
        int pid;
        
    }
    int size = rand() % 118 + 1; //Select random size from 1 - 118
}

int main() {
    //Set variables
    bool exit = false;
    mem.setSpace(118);

    //start of program
    while(!exit) {
        switch (menu()) {
            case 1: {
                initiate();
                break;
            }
            case 2:
                break;
            case 3:
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

