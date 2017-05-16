// DiningPhilosophers.c
//
// Matthew Feuling

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUMPHILOSOPHES 5
#define ONTHETABLE 0
#define OFFTHETABLE 1

int chopsticksOnTable[NUMPHILOSOPHES];

typedef struct {
	int philoID;
	pthread_mutex_t * chopstickMutex;
	pthread_cond_t * chopstickCond;
} philoParam;

void pickupChopsticks(philoParam * Params) {
	int reachDirection = Params->philoID % 2;

	// At this point, a philosopher knows whether to reach "right" (+1) or "left" (+0) first
	pthread_mutex_lock(&Params->chopstickMutex[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	while (chopsticksOnTable[(Params->philoID + reachDirection) % NUMPHILOSOPHES] != ONTHETABLE) {
		printf("\n(Ph:%i) Waiting on first chopstick..\n", Params->philoID);
		pthread_cond_wait(&Params->chopstickCond[(Params->philoID + reachDirection) % NUMPHILOSOPHES], &Params->chopstickMutex[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	}
	chopsticksOnTable[(Params->philoID + reachDirection) % NUMPHILOSOPHES] = OFFTHETABLE;
	pthread_mutex_unlock(&Params->chopstickMutex[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	// Got the chopstick to our left / right! Now we need to grab the opposite.
	printf("\n(Ph:%i) I grabbed my first chopstick: %i\n", Params->philoID, (Params->philoID + reachDirection) % NUMPHILOSOPHES);
	reachDirection = (reachDirection + 1) % 2;
	
	pthread_mutex_lock(&Params->chopstickMutex[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	while (chopsticksOnTable[(Params->philoID + reachDirection) % NUMPHILOSOPHES] != ONTHETABLE) {
		printf("\n(Ph:%i) Waiting on second chopstick..\n", Params->philoID);
		pthread_cond_wait(&Params->chopstickCond[(Params->philoID + reachDirection) % NUMPHILOSOPHES], &Params->chopstickMutex[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	}
	chopsticksOnTable[(Params->philoID + reachDirection) % NUMPHILOSOPHES] = OFFTHETABLE;
	pthread_mutex_unlock(&Params->chopstickMutex[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	// Got the second chopstick we needed. Time to eat and leave the mutex unlocked.	
	printf("\n(Ph:%i) I grabbed my second chopstick: %i.\n", Params->philoID, (Params->philoID + reachDirection) % NUMPHILOSOPHES);
}

void returnChopsticks(philoParam * Params) {
	//int reachDirection = Params->philoID % 2;
	int reachDirection = (Params->philoID + 1) % 2;	
	
	pthread_mutex_lock(&Params->chopstickMutex[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	chopsticksOnTable[(Params->philoID + reachDirection) % NUMPHILOSOPHES] = ONTHETABLE;
	pthread_cond_signal(&Params->chopstickCond[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	pthread_mutex_unlock(&Params->chopstickMutex[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	
	reachDirection = (reachDirection + 1) % 2;	
	pthread_mutex_lock(&Params->chopstickMutex[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	chopsticksOnTable[(Params->philoID + reachDirection) % NUMPHILOSOPHES] = ONTHETABLE;
	pthread_cond_signal(&Params->chopstickCond[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
	pthread_mutex_unlock(&Params->chopstickMutex[(Params->philoID + reachDirection) % NUMPHILOSOPHES]);
}


void philoFunction (philoParam * Params) {
	srand((time(NULL)) + Params->philoID);
	while (1) {
		printf("\n(Ph:%i) Thinking..\n", Params->philoID);
		sleep((rand() % 5) + 1);
		printf("\n(Ph:%i) Hungry!\n", Params->philoID);
		pickupChopsticks(Params);
		printf("\n(Ph:%i) I have my chopsticks and will eat for a few seconds..\n", Params->philoID);
		sleep((rand() % 5) + 1);
		returnChopsticks(Params);
		printf("\n(Ph:%i) I put my chopsticks back on the table.\n");
	}
}

int main (int argc, char * argv[]) {

	printf("\nSetting the table..\n");
	for (int i = 0; i < NUMPHILOSOPHES; i++) {
		chopsticksOnTable[i] = ONTHETABLE;
	}

	printf("\nMaking some rules for each chopstick..\n");
	pthread_mutex_t chopstickMutex[NUMPHILOSOPHES];
	pthread_cond_t chopstickCond[NUMPHILOSOPHES];
	for (int i = 0; i < NUMPHILOSOPHES; i++) {
		pthread_mutex_init(&chopstickMutex[i], NULL);
		pthread_cond_init(&chopstickCond[i], NULL);
	}

	printf("\nSitting the philosophers down to philosophize..\n");
	pthread_t Philosophers[NUMPHILOSOPHES];
	philoParam Params[NUMPHILOSOPHES];
	for (int i = 0; i < NUMPHILOSOPHES; i++) {
		Params[i].philoID = i;
		Params[i].chopstickMutex = chopstickMutex;
		Params[i].chopstickCond = chopstickCond;
		pthread_create(&Philosophers[i], NULL, (void *)(philoFunction), &Params[i]);
	}

	for (int i = 0; i < NUMPHILOSOPHES; i++) {
		pthread_join(Philosophers[i], NULL);
	}

	return EXIT_SUCCESS;
}
