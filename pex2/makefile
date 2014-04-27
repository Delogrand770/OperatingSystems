demo_shell: demo_shell.o list.o
	gcc demo_shell.o list.o -o demo_shell

demo_shell.o: demo_shell.c list.h
	gcc demo_shell.c -c

list.o: list.h list.c
	gcc list.c -c

prime: prime.o
	gcc prime.o -lm -pthread -o prime

prime.o: prime.c
	gcc prime.c -c

clean:
	-rm *.o
	-rm *~
	-rm demo_shell
	-rm prime
	clear

all: demo_shell prime
