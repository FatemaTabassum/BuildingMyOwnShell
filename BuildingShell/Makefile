CC=gcc
CFLAGS=-c -std=gnu99 -Wall -I.

all: main.o Parser.o ExecuteCommand.o Header.o
	$(CC) -o mybash main.o Parser.o ExecuteCommand.o Header.o
myls: MyLsImplement.o
	$(CC) -o myls MyLsImplement.o
MyLsImplement.o: MyLsImplement.c
	$(CC) $(CFLAGS) MyLsImplement.c
main.o: main.c
	$(CC) $(CFLAGS) main.c
Parser.o: Parser.c
	$(CC) $(CFLAGS) Parser.c
ExecuteCommand.o: ExecuteCommand.c
	$(CC) $(CFLAGS) -c ExecuteCommand.c
Header.o: Header.c
	$(CC) $(CFLAGS) -c Header.c
clean: rm -rf *o mybash 
