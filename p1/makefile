CC = gcc
CFLAGS = -Wall -Werror -pedantic
OBJECTS = 537ps.o read_process_info.o

537ps: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o 537ps

537ps.o: 537ps.c read_process_info.h
	$(CC) $(CFLAGS) -c 537ps.c

read_process_info.o: read_process_info.c read_process_info.h
	$(CC) $(CFLAGS) -c read_process_info.c


clean:
	rm 537ps $(OBJECTS) -f
