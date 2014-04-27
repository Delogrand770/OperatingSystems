#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define BUFFER_SIZE 5

sem_t chopstick[BUFFER_SIZE];
pthread_mutex_t mutex;
int food[BUFFER_SIZE];

void pickup_chopsticks(int);
void putdown_chopsticks(int);
void philosopher(int *philosopher_info);

int main(int argc, char *argv[]){
	pthread_t philosophers[BUFFER_SIZE];
	int philosopher_info[BUFFER_SIZE][1];
	pthread_mutex_init(&mutex, NULL); //Create mutex lock

	if (argc != 2){
		fprintf(stderr, "Invalid Number of Arguments\n\tUSAGE: ./dinf foodArg\n");
		exit(0);
	}
	int foodArg = atoi(argv[1]);

	int i;
	for (i = 0; i < BUFFER_SIZE; i++){
		if (sem_init(&chopstick[i], 0, 1) < 0) {
			fprintf(stderr, "ERROR: sem_init(&chopstick[%d], 0, 1)\n", i);
			exit(1);
		}
	}

	//Create a thread for each philosopher.
	for (i = 0; i < BUFFER_SIZE; i++){
		philosopher_info[i][0] = i;
		food[i] = foodArg;
		pthread_create(&philosophers[i], NULL, (void *(*)(void *)) &philosopher, &philosopher_info[i]);
	}

	//Wait for the BUFFER_SIZE to finish.
	for (i = 0; i < BUFFER_SIZE; i++){
		pthread_join(philosophers[i], NULL);
	}

	//Release semaphore resources.
	for (i = 0; i < BUFFER_SIZE; i++){
		sem_destroy(&chopstick[i]);
	}

	return 0;
}

void pickup_chopsticks(int n){
	if (n % 2 == 0) { //Even philosopher so left first
		pthread_mutex_lock(&mutex); //get mutex lock
		sem_wait(&chopstick[(n+1) % BUFFER_SIZE]);
		printf("Philosopher %d waiting for left chopstick\n", n);
		sem_wait(&chopstick[n]);
		printf("Philosopher %d waiting for right chopstick\n", n);
		pthread_mutex_unlock(&mutex); //release mutex lock
	}else{ //Odd philosopher so right first
		pthread_mutex_lock(&mutex); //get mutex lock
		sem_wait(&chopstick[n]);
		printf("Philosopher %d waiting for right chopstick\n", n);
		sem_wait(&chopstick[(n+1) % BUFFER_SIZE]);
		printf("Philosopher %d waiting for left chopstick\n", n);
		pthread_mutex_unlock(&mutex); //release mutex lock
	}
}

void putdown_chopsticks(int n){
	sem_post(&chopstick[n]);
	printf("Philosopher %d put down right chopstick\n", n);
	sem_post(&chopstick[(n+1) % BUFFER_SIZE]);
	printf("Philosopher %d put down left chopstick\n", n);

}

void philosopher(int *philosopher_info){
	int n = philosopher_info[0];
	while (food[n] > 0){
		//Thinking
		printf("Philosopher %d beginning to think\n", n);
		sleep(rand() % 10);

		//Hungry
		printf("Philosopher %d finished thinking and waiting for critical section\n", n);
        	pickup_chopsticks(n);

		//Eating
		printf("Philosopher %d starting to eat\n", n);
		food[n] = food[n] - 1;
		printf("\tPhilosopher %d has %d food left\n", n, food[n]);
		putdown_chopsticks(n);
	}

	//Done
	printf("Philosopher %d is done\n", n);
	pthread_exit(NULL);
}
