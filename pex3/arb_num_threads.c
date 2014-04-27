#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//in order to send in multiple parameters, I need to create an structure (arrays can work
//too if the parameters are all of the same type, but being able to name them in structure
//form is probably more intuitive than array indicies
struct params{
	int integer;
	char character;
};

//this is the function that will serve as my thread.  note that it returns a void* and
//expects a single parameter of type void*
void* print_stuff(void* p){
	
	//note that here I am type casting the pointer to the actual type I want.
	//I'm also making a variable for this so that I only have to do this once
	struct params* actual_parameters=(struct params*) p;

	printf("%d %c\n",actual_parameters->integer, actual_parameters->character);

	pthread_exit(NULL);
}

int main()
{
	//change this number and see how it works.  
	//you could likewise get this from the user or the command line
	int num_threads=5;

	//here I'm malloc-ing an array that will store the thread IDs
	pthread_t *thread=malloc(sizeof(pthread_t)*num_threads);

	//here I'm malloc-ing an array that will hold the parameters for each thread
	struct params* ps=malloc(sizeof(struct params) * num_threads);

	int i;//loop control variable

	//set parameters
	for (i=0;i<num_threads;i++){
		ps[i].integer=i;
		ps[i].character=65+i;//this is a C trick.  Characters are actually 
				     //integers interpreted by ASCII values.  65
				     //is the value of 'A', so i=0 -> 'A', i=1 -> B, etc.
	}
  
	//create threads.  I could have done this in the loop above, but oh well
	for (i=0;i<num_threads;i++){
		//notice that I am passing a pointer to the proper element of
		//the parameters array.
		pthread_create(&(thread[i]),NULL,&print_stuff,&(ps[i]));
	}

	//I must wait for all threads to exit
	for(i=0;i<num_threads;i++){
		pthread_join(thread[i],NULL);
	}

	printf("Back to single thread of execution\n");

	return EXIT_SUCCESS;
}

