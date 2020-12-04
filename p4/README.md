# CS537 Project 4
### Instruction: [http://pages.cs.wisc.edu/~bart/537/programs/program4.html](http://pages.cs.wisc.edu/~bart/537/programs/program4.html)
## Implementation
### Data structure
- process
- page
- process tree
- page tree
- statistics <br/>
In the page and process structures, we create some points to the next page/process to simulate the double linked list, including I/O list, runnable list, occupied frame list and free list.
### Modules
- input: The module pasrses the first pass of tracefile, including finding all the PID's, and marking the start and end of execution (first and last memory reference) of each process as well as reporting error for the traces that don't match the format. The frame size in memory is got by parsing the input arguments.

- basic: The module contains functions to reads in the trace data in the second pass and simulates the page replacement scheduling. 

- page replacement: The module contains the three different repalcement algorithms. They implement different page reference and replacement scheduling with the same interface.

### Results
- smallmix.addrtrace  tracefile <br/>

 | Algorithm     | "AMU"    |"ARP"	    |"TMR"| "TPI"	 |"RTime"	|"ExecTime"|
| --- | --- | --- | --- | --- | --- |  --- |
|FIFO |  0.372576|	0.000003|	1,555|	332|	664,000,002|	0.014 | 
|LRU   | 0.372576	|0.000003|	1,555|	332|	664,000,002|	0.026 |  
|CLOCK | 0.372576|	0.000003|	1,555|	332|	664,000,002|	0.017 | 

- 4000.addrtrace  tracefile  <br/>

| Algorithm     | "AMU"    |"ARP"	    |"TMR" |"TPI"	 |"RTime"	|"ExecTime"|
| --- | --- | --- | --- | --- | --- |  --- |
|FIFO  | 0.306535|	0.000012|	4,000	|167	|334,000,058|	0.052  |
|LRU   | 0.306535|	0.000012	|4,000	167	|334,000,058	|0.026 |
|CLOCK | 0.306535	|0.000012	|4,000	|167	|334,0000,58	|0.02 |
 
- bigmix.addrtrace  tracefile <br/>

| Algorithm     | "AMU"    |"ARP"	    |"TMR" |"TPI"	 |"RTime"	|"ExecTime"|
| --- | --- | --- | --- | --- | --- |--- |
|FIFO  | 0.999916|	0.000001	|4,534,297	|2,453,097	|4,906,194,201,210	|22.277|
|LRU  |  0.999915	|0.000001|	4,534,297	|2,449,409|	4,898,818,200,824	|22.598|
CLOCK | 0.999915|	0.000001|	4,534,297	|2,449,409	|4,898,818,200,824	|22.598 |

- 12million.addrtrace  tracefile <br/>

| Algorithm     | "AMU"    |"ARP"	    |"TMR"| "TPI"	 |"RTime"	|"ExecTime"|
| --- | --- | --- | --- | --- | --- | --- |
|FIFO  | 0.97782|	0.000628|	12,000,000|	9,559|	19,118,583,416|	37.636|
|LRU |   0.956025|	0.001125|	120,000,00|	5,337|	10,674,561,551|	35.955|
|CLOCK|  0.959527|	0.001043|	12,000,000|	5,756|	11,512,560,111|	37.013|

Our implementation includes O(logN) process table, O(logN) page table, O(1) disk queue, O(1) FIFO and LRU for both access and evict, O(1) clock access, O(N) clock evict. But when a process terminates, my implementation takes O(N) time to remove the corresponding page frames. I did not use the interval linked list either, which would probably be the issue that lowers the performance.


          
