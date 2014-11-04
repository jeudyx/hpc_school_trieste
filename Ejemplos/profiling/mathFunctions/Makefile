#------------------------------------------------------------------------------
# General Settings
#------------------------------------------------------------------------------

CC     =  icc 
CFLAGS = -O3 -g -pg 
LDLIBS =  

#------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------

OBJECTS = \
	mathfun.o \
	main.o


all: mathfun 

clean:
	rm -f mathfun $(OBJECTS)
	rm -f gmon.out perf.data* callgrind.*

mathfun: $(OBJECTS)
	$(CC) $(CFLAGS) -o mathfun $(OBJECTS) $(LDLIBS)

.PHONY: all clean
.SUFFIXES: .c .o

.c.o: 
	$(CC) $(CFLAGS) -c $<


#------------------------------------------------------------------------------
# Dependencies
#------------------------------------------------------------------------------

mathfun.o: mathfun.c mathfun.h
main.o: main.c mathfun.h

