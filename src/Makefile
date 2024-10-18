# src/Makefile

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
DEPS = rush.h utils.h commands.h path.h redirection.h
OBJ = main.o utils.o commands.o path.o redirection.o

# Target to build the rush executable
rush: $(OBJ)
	$(CC) $(CFLAGS) -o rush $(OBJ)

# Compile main.c
main.o: main.c $(DEPS)
	$(CC) $(CFLAGS) -c main.c

# Compile utils.c
utils.o: utils.c $(DEPS)
	$(CC) $(CFLAGS) -c utils.c

# Compile commands.c
commands.o: commands.c $(DEPS)
	$(CC) $(CFLAGS) -c commands.c

# Compile path.c
path.o: path.c $(DEPS)
	$(CC) $(CFLAGS) -c path.c

# Compile redirection.c
redirection.o: redirection.c $(DEPS)
	$(CC) $(CFLAGS) -c redirection.c

# Clean up compiled files
clean:
	rm -f *.o rush

