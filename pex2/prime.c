#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //link with -pthread
#include <math.h> //link with -lm

int divisor = 0;

struct arg_struct {
	long long num;
	int threadNum;
	int threadTotal;	
};

void* printmessage(void* arguments){
	struct arg_struct *args = (struct arg_struct *) arguments;
	long limit = ceil( sqrt( (double) args->num ) );
	long scale = ceil((double) limit / (double) args->threadTotal);
	long start = (args->threadNum - 1 ) * scale + 1;
	long end = args->threadNum * scale;

	//First thread must start at 2
	if (args->threadNum == 0){
		start = 2;
	}
	//Avoid extra checks
	if (args->threadNum == args->threadTotal){
		end = limit;
	}

	//printf("Thread: %d of %d | Thread Start: %lu | Thread End: %lu | Check To: %lu | Block Size: %lu\n", 
	//	args->threadNum, args->threadTotal, start, end, limit, scale);

	int i;
	//for (i = start; i <= end; i++){
	for (i = 2; i <= limit; i++){
		//End thread if a divisor has been found
		if (divisor != 0){
			return NULL;
		}

		if (((long) args->num % i) == 0){
			divisor = i;
			return NULL; //End thread because a divisor has been found
		}
	}
	
	wait(2);
	return NULL;
}

int main(int argc, char* argv[]){
   	struct arg_struct args[ atoi( argv[2] ) ];

	pthread_t threads[ atoi( argv[2] ) ];

	int i;
	for (i = 0; i < atoi( argv[2] ); i++){
		args[i].num = atoll( argv[1] );
		args[i].threadTotal = atoi( argv[2] );
   		args[i].threadNum = i + 1;
		pthread_create(&threads[ i ], NULL, &printmessage, (void*) &args[i]); 
	}
	for (i = 0; i < atoi( argv[2] ); i++){
		pthread_join(threads[ i ], NULL);
	}

	if (divisor == 0){
		printf("%llu is prime\n", (long long) atoll( argv[1] ));
	}else{
		printf("%llu is composite\n\tDivisor: %d\n", (long long) atoll( argv[1] ), divisor);
	}
}
