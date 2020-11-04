537make: main.o build_spec_rep.o parsing.o process_exec.o graph.o errorHandling.o
	gcc -Wall -Werror -o 537make main.o build_spec_rep.o parsing.o process_exec.o graph.o errorHandling.o

main.o: main.c build_spec_rep.h
	gcc -Wall -Werror -c main.c

build_spec_rep.o: build_spec_rep.c build_spec_rep.h parsing.h process_exec.h
	gcc -Wall -Werror -c build_spec_rep.c

process_exec.o: process_exec.c process_exec.h graph.h
	gcc -Wall -Werror -c process_exec.c

graph.o: graph.c graph.h errorHandling.h
	gcc -Wall -Werror -c graph.c

errorHandling.o: errorHandling.c errorHandling.h
	gcc -Wall -Werror -c errorHandling.c