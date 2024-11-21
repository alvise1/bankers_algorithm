CC = gcc
CFLAGS = -Wall -g

all: main

main: main.o file_reader.o resource_manager.o helpers.o
	$(CC) $(CFLAGS) -o main main.o file_reader.o resource_manager.o helpers.o

main.o: main.c file_reader.h resource_manager.h helpers.h
	$(CC) $(CFLAGS) -c main.c

file_reader.o: file_reader.c file_reader.h
	$(CC) $(CFLAGS) -c file_reader.c

resource_manager.o: resource_manager.c resource_manager.h file_reader.h
	$(CC) $(CFLAGS) -c resource_manager.c

helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) -c helpers.c

clean:
	rm -f *.o main
