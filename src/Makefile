CC = gcc
CFLAGS = -Wall -Werror -std=c99

OBJECTS = main.o parser.o executor.o path_manager.o utilities.o

rush: $(OBJECTS)
	$(CC) $(CFLAGS)	-o	rush $(OBJECTS)

main.o: main.c rush.h
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c rush.h
	$(CC) $(CFLAGS) -c parser.c

executor.o: executor.c rush.h
	$(CC) $(CFLAGS) -c executor.c

path_manager.o: path_manager.c rush.h
	$(CC) $(CFLAGS) -c path_manager.c

utilities.o: utilities.c rush.h
	$(CC) $(CFLAGS) -c utilities.c

clean:
	rm -f rush *.o