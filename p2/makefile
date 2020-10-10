CC      = gcc
WARNING_FLAGS  = -Wall -Wextra -Werror
OPTS = -pthread
EXE = prodcom
SCAN_BUILD_DIR = scan-build-out
OBJECTS = main.o threads.o Queue.o errorHandling.o stats.o



prodcom: $(OBJECTS)
	$(CC) $(OPTS) $(WARNING_FLAGS) -o $(EXE) $(OBJECTS)

main.o: main.c threads.h
	$(CC) $(OPTS) $(WARNING_FLAGS) -c main.c

threads.o: threads.c threads.h Queue.h
	$(CC) $(OPTS) $(WARNING_FLAGS) -c threads.c

Queue.o: Queue.c Queue.h errorHandling.h stats.h
	$(CC) $(OPTS) $(WARNING_FLAGS) -c Queue.c

errorHandling.o: errorHandling.c errorHandling.h
	$(CC) $(OPTS) $(WARNING_FLAGS) -c errorHandling.c

stats.o: stats.c stats.h
	$(CC) $(OPTS) $(WARNING_FLAGS) -c stats.c

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)
