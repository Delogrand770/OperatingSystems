pc: pc.o
	gcc pc.o -pthread -o pc

pc.o: pc.c buffer.h
	gcc pc.c -c

dinf: dinf.o
	gcc dinf.o -pthread -o dinf

dinf.o: dinf.c
	gcc dinf.c -c

clean:
	-rm *.o
	-rm *~
	-rm pc
	-rm dinf
	clear

all: pc dinf
