CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -pthread
OBJECTS = prodcom.o threads.o Queue.o

prodcom: $(OBJECTS)
	$(CC) $(CFLAGS) -o prodcom $(OBJECTS)

prodcom.o: prodcom.c threads.h Queue.h
	$(CC) $(CFLAGS) -c prodcom.c

threads.o: threads.c threads.h Queue.h
	$(CC) $(CFLAGS) -c threads.c

Queue.o: Queue.c Queue.h
	$(CC) $(CFLAGS) -c Queue.c

clean:
	rm -f $(OBJECTS) prodcom
