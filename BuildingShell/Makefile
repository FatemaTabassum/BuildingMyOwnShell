CC = gcc
CFLAGS = -I.
DEPS = Header.h Parser.h ExecuteCommand.h

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

myOwnBash: main.o Parser.o ExecuteCommand.o Header.o
		$(CC) -o myOwnBash main.o Parser.o ExecuteCommand.o Header.o

# myOwnBash: main.c Parser.c ExecuteCommand.c Header.c
# 	$(CC) -o myOwnBash main.c Parser.c ExecuteCommand.c Header.c -I.
