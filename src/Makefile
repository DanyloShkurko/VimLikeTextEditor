CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lncurses
TARGET=../bin/

all: prepare text_editor 

prepare:
	mkdir -p $(TARGET)

text_editor: main.o file_manager.o
	$(CC) $(TARGET)main.o $(TARGET)file_manager.o -o $(TARGET)text_editor $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)main.o

file_manager.o: file_manager/file_manager.c
	$(CC) $(CFLAGS) file_manager/file_manager.c -o $(TARGET)file_manager.o

clean:
	rm -rf $(TARGET)*.o $(TARGET)text_editor