CC = gcc
CFLAGS = -Wall	-Werror	-std=gnu99

TARGET = rush

OBJS = main.o parser.o executor.o path.o error.o

$(TARGET): $(OBJS)
	$(CC)	$(CFLAGS)	-o	$(TARGET)	$(OBJS)  # Ensure this line uses a tab

main.o: main.c
	$(CC)	$(CFLAGS)	-c	main.c  # Ensure this line uses a tab

parser.o: parser.c
	$(CC)	$(CFLAGS)	-c	parser.c  # Ensure this line uses a tab

executor.o: executor.c
	$(CC)	$(CFLAGS)	-c	executor.c  # Ensure this line uses a tab

path.o: path.c
	$(CC)	$(CFLAGS)	-c	path.c  # Ensure this line uses a tab

error.o: error.c
	$(CC)	$(CFLAGS)	-c	error.c  # Ensure this line uses a tab

clean:
	rm -f $(OBJS) $(TARGET)  # Ensure this line uses a tab