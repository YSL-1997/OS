During lecture today it was mentioned there could be a need for an entire module to keep track of queue statistics. 
I found this very confusing as it feels like you could just keep track of them within the queue methods and call a 
method within the queue module to print statistics for that queue before exiting? Additionally is there any synchronization 
that needs to be associated with printing the statistics if it is assumed the program is done with the queues anyway? 

Good question.

If I were writing this code (and have written many programs like this one), I would have a statistics "object" ... which in C, would be struct.

This struct (defined with a typedef) would be in the .h file of its own module with init, update, and print functions in the .c file.

In the Queue struct, I would in this this statistics struct as a field (data member).

--bart
