CC = gcc
CFLAGS = -O3 -Wall -Wextra -fsanitize=address -g 
EXECUTABLE_C = out/memoised.o

default: ; # Options: make ccomp, make crun, make clean

ccomp:
	$(CC) $(CFLAGS) src/memoised/memoised.c ../fuzzer/src/c/fuzzer.c  -o $(EXECUTABLE_C)

crun:
	$(EXECUTABLE_C)

clean:
	rm -rf out/*