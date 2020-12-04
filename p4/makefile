all: 537pfsim-fifo 537pfsim-lru 537pfsim-clock

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
	gcc -Wall -Wextra -pedantic -c main.c

basic.o: basic.c input.h process.h tsearch.h errorHandling.h page.h \
		  basic.h statistics.h
	gcc -Wall -Wextra -pedantic -c basic.c

clock.o: clock.c pageAlgorithms.h page.h
	gcc -Wall -Wextra -pedantic -c clock.c

fifo.o: fifo.c pageAlgorithms.h page.h
	gcc -Wall -Wextra -pedantic -c fifo.c

lru.o: lru.c pageAlgorithms.h page.h
	gcc -Wall -Wextra -pedantic -c lru.c

input.o: input.c input.h process.h tsearch.h errorHandling.h
	gcc -Wall -Wextra -pedantic -c input.c

tsearch.o: tsearch.c tsearch.h process.h page.h
	gcc -Wall -Wextra -pedantic -c tsearch.c

statistics.o: statistics.c statistics.h
	gcc -Wall -Wextra -pedantic -c statistics.c

process.o: process.c process.h
	gcc -Wall -Wextra -pedantic -c process.c

page.o: page.c page.h
	gcc -Wall -Wextra -pedantic -c page.c

errorHandling.o: errorHandling.c errorHandling.h
	gcc -Wall -Wextra -pedantic -c errorHandling.c

clean:
	rm -f 537pfsim-fifo 537pfsim-lru 537pfsim-clock *.o
