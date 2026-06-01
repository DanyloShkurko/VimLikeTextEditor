CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lncurses
TARGET=../bin/

all: prepare text_editor

prepare:
	mkdir -p $(TARGET)

text_editor: main.o file_manager.o num_bar_manager.o mode_handler.o changes_manager.o buffer.o
	$(CC) $(TARGET)main.o $(TARGET)file_manager.o $(TARGET)num_bar_manager.o $(TARGET)mode_handler.o $(TARGET)changes_manager.o $(TARGET)buffer.o -o $(TARGET)text_editor $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)main.o

file_manager.o: file_manager/file_manager.c
	$(CC) $(CFLAGS) file_manager/file_manager.c -o $(TARGET)file_manager.o

num_bar_manager.o: num_bar_manager/num_bar_manager.c
	$(CC) $(CFLAGS) num_bar_manager/num_bar_manager.c -o $(TARGET)num_bar_manager.o

mode_handler.o: mode_handler/mode_handler.c
	$(CC) $(CFLAGS) mode_handler/mode_handler.c -o $(TARGET)mode_handler.o

changes_manager.o: changes/changes_manager.c
	$(CC) $(CFLAGS) changes/changes_manager.c -o $(TARGET)changes_manager.o

buffer.o: buffer/buffer.c
	$(CC) $(CFLAGS) buffer/buffer.c -o $(TARGET)buffer.o

clean:
	rm -rf $(TARGET)*.o $(TARGET)text_editor
