CC = gcc
CFLAGS = 

all: runsim testsim

runsim: main.c runsim.h
	$(CC) $(CFLAGS) main.c -o runsim

testsim: testsim.c runsim.h
	$(CC) $(CFLAGS) testsim.c -o testsim

clean:
	rm -rf runsim testsim *.txt