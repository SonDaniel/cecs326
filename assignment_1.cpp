#include <iostream>
#include <stdlib.h>
#include <queue>
using namespace std;

class MemoryBlockTable {
    public:
        void setSpace(int x) {space = x;}
        int getSpace() {return space;}
    private:
        int space;
        bool mem [128];
};

MemoryBlockTable mem;
int pid = 0;

int menu() {
    int choice;
    cout << endl << endl;
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
    cin >> choice;
    return choice;
}

int main() {
    bool exit = false;
    while(!exit) {
        switch (menu()) {
            case 1: {
                
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

