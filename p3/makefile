SCAN_BUILD_DIR = scan-build-out

537make: main.o build_spec_rep.o parsing.o process_exec.o graph.o \
errorHandling.o
	gcc -Wall -Wextra -pedantic -o 537make main.o build_spec_rep.o \
parsing.o process_exec.o graph.o errorHandling.o

main.o: main.c build_spec_rep.h
	gcc -Wall -Wextra -pedantic -c main.c

build_spec_rep.o: build_spec_rep.c build_spec_rep.h parsing.h process_exec.h
	gcc -Wall -Wextra -pedantic -c build_spec_rep.c

process_exec.o: process_exec.c process_exec.h graph.h parsing.h
	gcc -Wall -Wextra -pedantic -c process_exec.c

graph.o: graph.c graph.h errorHandling.h
	gcc -Wall -Wextra -pedantic -c graph.c

parsing.o: parsing.c parsing.h graph.h
	gcc -Wall -Wextra -pedantic -c parsing.c

errorHandling.o: errorHandling.c errorHandling.h
	gcc -Wall -Wextra -pedantic -c errorHandling.c

clean:
	rm -f 537make *.o
	rm -rf $(SCAN_BUILD_DIR)

# run the Clang Static Analyzer

scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make