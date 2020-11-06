# CS537 Programming Assignment #3: Make

There are several goals for this assignment.
- Learn about creating new processes, loading new programs, and waiting for processes to complete in Linux.
- Get experience with poorly formed input and handling the errors gracefully. (Defensive programming.)
- Learning a few more useful Linux kernel calls, such as fstat().
Your assignment is to write a simple version of the make command. Your version of make, called 537make will be executed from a command line. It will read a a "makefile" and following the specifications in the makefile as to which commands need to be executed. So, you will implement a small but quite useful subset of the standard Linux make program.
