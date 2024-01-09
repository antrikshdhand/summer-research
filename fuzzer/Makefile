CC = gcc
CFLAGS = -g -O1 -O3 -fsanitize=address 
TARGET = fuzzer.o

all: $(TARGET)

$(TARGET): fuzzer.c
	$(CC) $(CFLAGS) fuzzer.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
