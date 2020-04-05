#C for object files, E for executables, A for all, L for lib flags
CFLAGS=-c -std=c99 -Wall -pedantic
EFLAGS=-Wall -pedantic
AFLAGS=-std=c99 -Wall -pedantic 
LFLAGS=-lpthread #-lm

ass7: testbed7.o assignment7.c assignment7.h
	cc -o ass7 $(EFLAGS) testbed7.o assignment7.c $(LFLAGS)

testbed7.o: testbed7.c assignment7.h
	cc $(CFLAGS) testbed7.c

all: 
	cc -o ass7 $(AFLAGS) testbed7.c assignment7.c $(LFLAGS)

clean:
	rm -f testbed7.o && rm -f ass7 && rm -f -R ass7.dSYM

run: ass7
	./ass7



cc -o ass7 -std=c99 -Wall -pedantic testbed7.c assignment7.c 