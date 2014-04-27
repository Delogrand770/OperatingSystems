#comment
pex: main.o history.o makefile
	gcc main.o history.o -o pex

main.o: main.c history.h makefile
	gcc -c main.c

history.o: history.c history.h makefile
	gcc -c history.c

clean:
	-rm *.o
	-rm *~
	-rm pex
