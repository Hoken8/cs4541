CC = gcc
CFLAGS = -std=c99 -Wall
TARGET = simple_c_program

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

run:
	./$(TARGET)	

clean:
	rm -rf *o $(TARGET)