CC = icc
OPT4 = -O3 

default: mountain.x

mountain.x: mountain.c fcyc2.c clock.c
	$(CC)  $(OPT4) -o $@ mountain.c fcyc2.c clock.c 

clean:
	rm -f mountain *.o *~ mountain-opt*


