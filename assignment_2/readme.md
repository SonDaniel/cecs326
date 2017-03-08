CECS 326 Assignment 2
Due: March 7, 2017

Cooperating processes need to communicate between them.  One way Linux supports interprocess communication is message queue.  A message queue must first be acquired from the operating system by calling msgget.  Control operations, e.g., remove, can be performed on an existing message queue by calling msgctl.  Processes with appropriate permissions may send and/or receive messages via the message queue by calling msgsnd and msgrcv.  Please consult the man pages of these system calls for details.
For this assignment you need to write three C++ programs named master.cpp, sender.cpp, and receiver.cpp, which should be compiled into master, sender, and receiver, respectively.  Together they should do the following:

The master process should first acquire a message queue from the operating system, followed by creating two child processes, with one to execute sender and the other to execute receiver.  Identify sender and receiver by outputting their process IDs (PIDs).  master then waits for both child processes to return.

The sender process should prompt user for a line of input, then send the input line to receiver via the message queue created by master.  Make sure that the prompt identifies the process making the prompt is sender along with its PID.

The receiver process retrieves a message from the message queue, and output the message on the screen.  Make sure the output identifies that the line is printed by receiver along with PID.

The program must run successfully on a Linux machine in the CECS Lab.

Do the following for this assignment:
1. Compile and run the use_msgQ.cpp program shown in the Processes lecture notes.  Try to understand what the program does, and annotate the program with adequate amount of comments throughout the program to explain what the program does.
2. Develop three C++ programs (master.cpp, sender.cpp, and receiver.cpp) as described above.  Make sure your programs are adequately commented.
3. Submit on BeachBoard and a hard copy of the annotated use_msgQ.cpp and your three C++ programs, with a cover page that provide course #, assignment #, date, name, email address, and a description of all 4 programs.
4. The programs must be properly formatted and adequately commented to enhance readability and understanding.
5. Demonstrate your program on a Linux machine in the Lab and explain details of your implementation.