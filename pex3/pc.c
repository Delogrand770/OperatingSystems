#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <syscall.h>
#include "buffer.h"

pthread_mutex_t mutex; //mutex lock
sem_t full, empty; //semaphores

pthread_t tid; //thread id
pthread_attr_t attr; //thread attrs

buffer_item buffer[BUFFER_SIZE]; //buffer
int counter; //buffer counter

void *producer(void *param);
void *consumer(void *param);
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void print_buffer();

int main(int argc, char *argv[]){
	if (argc != 4){
		fprintf(stderr, "Invalid Number of Arguments.\n\tUSAGE: ./pc sleepArg producerArg consumerArg\n");
		exit(0);
	}

	int i;
	for (i = 0; i < BUFFER_SIZE; i++){
		buffer[i] = -1;
	}
	int sleepArg = atoi(argv[1]);
	int producerArg = atoi(argv[2]);
	int consumerArg = atoi(argv[3]);

	pthread_mutex_init(&mutex, NULL); //Create mutex lock

	//Create semaphores and initialize
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);

	pthread_attr_init(&attr); //Get default attributes

	counter = 0;

	//Create producer threads.
	for (i = 0; i < producerArg; i++){
		pthread_create(&tid, &attr, producer, NULL);
	}

	//Create consumer threads.
	for (i = 0; i < consumerArg; i++){
		pthread_create(&tid, &attr, consumer, NULL);
	}

	//Sleep
	sleep(sleepArg);

	//Exit
	printf("Program Exited.\n");
	fflush(stdout);
	exit(0);
}

void *producer(void *param){
	buffer_item item;

	while (1){
		sleep(rand() % 10); //sleep for random period of time

      		item = rand(); //generate random number

		printf("Thread %d waiting for an empty space\n", syscall(SYS_gettid));
		fflush(stdout);
		sem_wait(&empty); //get empty lock

		printf("Thread %d waiting for critical section\n", syscall(SYS_gettid));
		fflush(stdout);
		pthread_mutex_lock(&mutex); //get mutex lock

		printf("Thread %d has entered critical section\n", syscall(SYS_gettid));
		fflush(stdout);

		if (insert_item(item)){
			fprintf(stderr, " Producer report error condition\n");
		}else{
			printf("producer produced %d\n", item);
			fflush(stdout);
		}

		pthread_mutex_unlock(&mutex); //release mutex lock
		printf("Thread %d has exited critical section\n", syscall(SYS_gettid));
		fflush(stdout);

		sem_post(&full); //signal full
		printf("Thread %d has signaled for a full space\n", syscall(SYS_gettid));
		fflush(stdout);
	}
}

void *consumer(void *param) {
	buffer_item item;

	while (1) {
		sleep(rand() % 10); //sleep for random period of time

		printf("Thread %d waiting for a full space\n", syscall(SYS_gettid));
		fflush(stdout);
		sem_wait(&full); //get full lock
		

		printf("Thread %d waiting for critical section\n", syscall(SYS_gettid));
		fflush(stdout);
		pthread_mutex_lock(&mutex); //get mutex lock
		

		printf("Thread %d has entered critical section\n", syscall(SYS_gettid));
		fflush(stdout);

		if (remove_item(&item)){
			fprintf(stderr, "Consumer report error condition\n");
		}else{
			printf("consumer consumed %d\n", item);
			fflush(stdout);
		}
		
		pthread_mutex_unlock(&mutex); //release mutex lock
		printf("Thread %d has exited critical section\n", syscall(SYS_gettid));
		fflush(stdout);

		sem_post(&empty); //signal empty		
		printf("Thread %d has signaled for an empty space\n", syscall(SYS_gettid));
		fflush(stdout);
	}
}

int insert_item(buffer_item item){
	buffer[in] = item;
	in = (in + 1) % BUFFER_SIZE;
	print_buffer();
	return 0;
}

int remove_item(buffer_item *item){
	*item = buffer[out];
	buffer[out] = -1;
	out = (out + 1) % BUFFER_SIZE;
	print_buffer();
	return 0;
}

void print_buffer(){
	int i;
	printf("\t BUFFER:");fflush(stdout); 
	for (i=0;i<BUFFER_SIZE;i++){
		printf(" %10d",buffer[i]);fflush(stdout); 
	}
	printf("\n");fflush(stdout);fflush(stdout); 
}
