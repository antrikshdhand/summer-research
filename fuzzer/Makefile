CC = gcc
CFLAGS = -g -O1 -O3 -fsanitize=address -lprofiler -L/opt/homebrew/Cellar/gperftools/2.15/lib -I/opt/homebrew/Cellar/gperftools/2.15/include
TARGET = fuzzer.o

all: $(TARGET)

$(TARGET): fuzzer.c
	$(CC) $(CFLAGS) fuzzer.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
