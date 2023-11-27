CC = gcc
CFLAGS = -std=c99 -Wall
TARGET = hsim

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

run:
	./$(TARGET)	

clean:
	rm -rf *o $(TARGET)