SCAN_BUILD_DIR = scan-build-out

537pfsim-fifo: main.o fifo.o basic.o input.o tsearch.o \
statistics.o process.o page.o errorHandling.o
	gcc -Wall -Wextra -pedantic -o 537pfsim-fifo main.o fifo.o basic.o \
	input.o tsearch.o statistics.o process.o page.o errorHandling.o

537pfsim-lru: main.o lru.o basic.o input.o tsearch.o \
statistics.o process.o page.o errorHandling.o
	gcc -Wall -Wextra -pedantic -o 537pfsim-lru main.o lru.o basic.o \
	input.o tsearch.o statistics.o process.o page.o errorHandling.o

537pfsim-clock: main.o clock.o basic.o input.o tsearch.o \
statistics.o process.o page.o errorHandling.o
	gcc -Wall -Wextra -pedantic -o 537pfsim-clock main.o clock.o basic.o \
	input.o tsearch.o statistics.o process.o page.o errorHandling.o

main.o: main.c basic.h
	gcc -Wall -Wextra -pedantic -c -pg main.c

basic.o: basic.c input.h process.h tsearch.h errorHandling.h page.h \
		  basic.h statistics.h
	gcc -Wall -Wextra -pedantic -c -pg basic.c

clock.o: clock.c pageAlgorithms.h page.h
	gcc -Wall -Wextra -pedantic -c -pg clock.c

fifo.o: fifo.c pageAlgorithms.h page.h
	gcc -Wall -Wextra -pedantic -c -pg fifo.c

lru.o: lru.c pageAlgorithms.h page.h
	gcc -Wall -Wextra -pedantic -c -pg lru.c

input.o: input.c input.h process.h tsearch.h errorHandling.h
	gcc -Wall -Wextra -pedantic -c -pg input.c

tsearch.o: tsearch.c tsearch.h process.h page.h
	gcc -Wall -Wextra -pedantic -c -pg tsearch.c

statistics.o: statistics.c statistics.h
	gcc -Wall -Wextra -pedantic -c -pg statistics.c

process.o: process.c process.h
	gcc -Wall -Wextra -pedantic -c -pg process.c

page.o: page.c page.h
	gcc -Wall -Wextra -pedantic -c -pg page.c

errorHandling.o: errorHandling.c errorHandling.h
	gcc -Wall -Wextra -pedantic -c -pg errorHandling.c

clean:
	rm -f 537pfsim-fifo 537pfsim-lru 537pfsim-clock *.o
	rm -rf $(SCAN_BUILD_DIR)

# run the Clang Static Analyzer

scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make