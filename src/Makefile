CC = gcc
CFLAGS = -Wall -Werror -std=c11
DEPS = parser.h executor.h path_manager.h utils.h
OBJ = main.o parser.o executor.o path_manager.o utils.o

all: rush

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

rush: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o rush