
prodcom: main.o threads.o Queue.o errorHandling.o stats.o
	gcc -pthread -Wall    -Wextra -Werror -o prodcom main.o threads.o Queue.o errorHandling.o stats.o

main.o :main.c threads.h
	gcc -pthread -Wall -Wextra -Werror -c main.c

threads.o: threads.c threads.h Queue.h
	gcc -pthread -Wall -Wextra -Werror -c threads.c

Queue.o: Queue.c Queue.h errorHandling.h stats.h
	gcc -pthread -Wall -Wextra -Werror -c Queue.c

errorHandling.o: errorHandling.c errorHandling.h
	gcc -pthread -Wall -Wextra -Werror -c errorHandling.c

stats.o: stats.c stats.h
	gcc -pthread -Wall -Wextra -Werror -c stats.c



#clean:
#	rm -f prodcom *.o
#	rm -rf scan-build-out

# run the Clang Static Analyzer

#scan-build: clean
#	scan-build -o scan-build-out make

# view the one scan available using chrome
#scan-view: scan-build
#	firefox -new-window scan-build-out/*/index.html
