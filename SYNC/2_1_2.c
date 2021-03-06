// NEVER DEADLOCK

#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h>

#define N 5 
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define LEFT (phnum + 4) % N 
#define RIGHT (phnum + 1) % N 

int state[N]; 
int phil[N] = { 0, 1, 2, 3, 4 }; 

sem_t mutex; 
sem_t S[N]; 
sem_t forks[N]; 

void test(int phnum) 
{ /*
	if (state[phnum] == HUNGRY 
		&& state[LEFT] != EATING 
		&& state[RIGHT] != EATING) { 
		// state that eating 
		state[phnum] = EATING; 
*/
		printf("Philosopher %d takes fork %d and %d\n", 
					phnum + 1, LEFT + 1, phnum + 1); 

		printf("Philosopher %d is Eating\n", phnum + 1); 
		sleep(2); 

		// sem_post(&S[phnum]) has no effect 
		// during takefork 
		// used to wake up hungry philosophers 
		// during putfork 
		sem_post(&S[phnum]); 
	 
} 

// take up chopsticks 
void take_fork(int phnum) 
{ 
   int low, high;
    if(phnum != 0) {
    	low = LEFT;
    	high = phnum;
    }
    else {
    	low = phnum;
    	high = LEFT;
    }
	
	sem_wait(&forks[low]); 
	sleep(1); 
	sem_wait(&forks[high]); 

	// eat if neighbours are not eating 
	test(phnum); 

} 



// put down chopsticks 
void put_fork(int phnum) 
{ 
int low, high;
    if(phnum != 0) {
    	low = LEFT;
    	high = phnum;
    }
    else {
    	low = phnum;
    	high = LEFT;
    }


	printf("Philosopher %d putting fork %d and %d down\n", 
		phnum + 1, LEFT + 1, phnum + 1); 
	printf("Philosopher %d is thinking\n", phnum + 1); 

	sem_post(&forks[high]); 
	sem_post(&forks[low]);
} 

void* philospher(void* num) 
{ 

	while (1) { 

		int* i = num; 

		sleep(1); 

		take_fork(*i); 

		sleep(0); 

		put_fork(*i); 
	} 
} 

int main() 
{ 

	int i; 
	pthread_t thread_id[N]; 

	// initialize the mutexes 
	sem_init(&mutex, 0, 1); 

	for (i = 0; i < N; i++) 

		sem_init(&S[i], 0, 0); 
	
	for (i = 0; i < N; i++) 

		sem_init(&forks[i], 0, 1); 

	for (i = 0; i < N; i++) { 

		// create philosopher processes 
		pthread_create(&thread_id[i], NULL, 
					philospher, &phil[i]); 

		printf("Philosopher %d is thinking\n", i + 1); 
	} 

	for (i = 0; i < N; i++) 

		pthread_join(thread_id[i], NULL); 
} 
