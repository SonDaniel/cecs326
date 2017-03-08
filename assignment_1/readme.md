CECS 326 Assignment 1
Due: February 14, 2017

This assignment serves as a review of C/C++ programming that includes dynamic memory allocation, pointer, struct, queue, array, and random number generation.  The program is to implement much simplified operations for part of the process and memory management in operating systems.
When your program executes, it should first create a memory block table (MBT) data structure, which consists of an integer and a Boolean array that maintain the state of the memory.  Assume the system has a total of 128 blocks and the first 10 blocks are reserved for the operating system use.  The program should then display a menu with four choices and respond to each choice as follows:
1. Initiate a process: 
    Response: 	allocate dynamic memory for a process control block (PCB)
		obtain a unique process ID (PID) and save it in the PCB
		generate a random number for required memory blocks
		search MBT for free blocks, if insufficient free blocks are available then display 
    error message and return to menu
		allocate memory blocks (only a simulation by designating required number of blocks in 
		    MBT as “allocated”)
		allocate dynamic memory for a page table and store allocated blocks in it
		save the pointer to page table in PCB
		insert PCB in ready queue
output page table and MBT to enable verification, and return to menu

2. Print all processes in ready queue:
    Response:	output PID for each process in ready queue and return to menu

3. Terminate process with a specific PID:
    Response:	search through ready queue for the given PID, output error message if not found
		deallocate memory blocks allocated for the process (only a simulation by designating
    those assigned blocks to this process as “free”)
		delete page table for the process
		delete PCB for the process and return to menu

4. Exit:
Response:	delete all dynamically allocated memory and end program execution

The program may be implemented in C or C++, and you may use your preferred platform for its development.  However, it must run successfully on a Linux machine in the CECS Lab.

Submit:
A hard copy of the program, with a cover page that provide course #, assignment #, date, name, email address, and program description.
The program must be properly formatted and adequately commented to enhance readability and understanding.
Demonstrate your program on a Linux machine in the Lab and explain details of your implementation.