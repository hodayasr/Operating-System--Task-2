// LIVELOCK


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
{ 
	if (state[phnum] == HUNGRY 
		&& state[LEFT] != EATING 
		&& state[RIGHT] != EATING) { 
		// state that eating 
		state[phnum] = EATING; 

		sleep(2); 

		printf("Philosopher %d is Eating\n", phnum + 1); 

		// sem_post(&S[phnum]) has no effect 
		// during takefork 
		// used to wake up hungry philosophers 
		// during putfork 
		sem_post(&S[phnum]); 
	} 
} 

// take up chopsticks 
void take_fork(int phnum) 
{ 
    
	sem_wait(&forks[phnum]); 
	sleep(1); 
	printf("Philosopher %d takes fork %d\n", phnum + 1, phnum + 1);
	int flag = 1, counter = 0, val;
	while(flag){
		sem_getvalue(&forks[LEFT], &val);
		if(val == 0) {
			sleep(1); 
			counter++;
			if(counter == 5) {
				sem_post(&forks[phnum]); 
				// put down the right fork
				printf("Philosopher %d putting fork %d down\n", phnum + 1, phnum + 1); 
				sleep(5); 
				counter = 0;
				// take the right fork again
				sem_wait(&forks[phnum]);
				printf("Philosopher %d takes fork %d\n", phnum + 1, phnum + 1);
				sleep(1); 
			}	
		}
		else flag = 0;
	}
	sem_wait(&forks[LEFT]); 
	printf("Philosopher %d takes fork %d\n", phnum + 1, LEFT + 1); 




	// state that hungry 
	state[phnum] = HUNGRY; 

	printf("Philosopher %d is Hungry\n", phnum + 1); 

	// eat if neighbours are not eating 
	test(phnum); 

	//sem_post(&mutex);
	sem_post(&forks[phnum]); 
	sem_post(&forks[LEFT]); 

	// if unable to eat wait to be signalled 
	sem_wait(&S[phnum]); 

	sleep(1); 
} 

// put down chopsticks 
void put_fork(int phnum) 
{ 

	//sem_wait(&mutex); 
	sem_wait(&forks[phnum]); 
	sleep(1); 
	sem_wait(&forks[LEFT]); 

	// state that thinking 
	state[phnum] = THINKING; 

	printf("Philosopher %d putting fork %d and %d down\n", 
		phnum + 1, LEFT + 1, phnum + 1); 
	printf("Philosopher %d is thinking\n", phnum + 1); 

	test(LEFT); 
	test(RIGHT); 

	//sem_post(&mutex);
	sem_post(&forks[phnum]); 
	sem_post(&forks[LEFT]); 
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