# CS537 Programming Assignment #3: Make

There are several goals for this assignment.
- Learn about creating new processes, loading new programs, and waiting for processes to complete in Linux.
- Get experience with poorly formed input and handling the errors gracefully. (Defensive programming.)
- Learning a few more useful Linux kernel calls, such as fstat().
Your assignment is to write a simple version of the make command. Your version of make, called 537make will be executed from a command line. It will read a a "makefile" and following the specifications in the makefile as to which commands need to be executed. So, you will implement a small but quite useful subset of the standard Linux make program.

## Project Instruction
The project requirements are in [http://pages.cs.wisc.edu/~bart/537/programs/program3.html](http://pages.cs.wisc.edu/~bart/537/programs/program3.html)
## Programe Structure
The modules are designed as:
- Text parsing: This module contains functions that help to parse lines in the makefile. It splits a line into an array of strings, checking whether the line begins with a tab or regular character, and filters out blank lines, see [parsing.c](https://github.com/YSL-1997/OS/blob/master/p3/parsing.c)
- Build specification graph: This module has functions for both building the graph that represents the dependences between build specifications, and traversing the graph in a bottom-up order to evaluate the specifications (effectively a post-order traversal), see [graph.c](https://github.com/YSL-1997/OS/blob/master/p3/graph.c)
- Build specification representation: This module contains the basic build specification abstraction. It allows to create, update, and access a build specification, see [build_spec_rep.c](https://github.com/YSL-1997/OS/blob/master/p3/build_spec_rep.c)
- Process creation and program execution: This module is responsible for running each build command in a new process, waiting for its completion, and getting the return code, see [process_exec.c](https://github.com/YSL-1997/OS/blob/master/p3/process_exec.c)
- Handle system function error: This module help tp check the return value of all library and system calls, see [errorHandling.c](https://github.com/YSL-1997/OS/blob/master/p3/errorHandling.c)
