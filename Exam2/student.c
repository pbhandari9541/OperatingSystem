#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/queue.h>
#define MAX_SLEEP_TIME	5
#define NUM_OF_STUDENTS	 5
#define NUM_OF_SEATS	3
void rnWait();
void *student_loop(void *param);
void *ta_loop();
int student_id[NUM_OF_STUDENTS];
int allDone = 0;
int count = 0;
int seat[NUM_OF_STUDENTS];

// ta allow the TA to sleep untill the student arrives
sem_t taSleep;

// mutually exclusive area
sem_t  tAseat;


//make sure the student wait untill the Ta is done;
sem_t onChair;

//checks to see whether TA is with student or not
sem_t mutex;

int main(void)
{
	int i;
	int j;
	int k;
	pthread_t ta;
	pthread_t students[NUM_OF_STUDENTS];

	//initialize
	for(j = 0; j < NUM_OF_STUDENTS; j++){
		student_id[j] = j;
	}

    	// Initialize the semaphores with initial values...
    	sem_init(&onChair, 0, NUM_OF_SEATS);
    	sem_init(&tAseat, 0, 1);
    	sem_init(&taSleep, 0, 0);
    	sem_init(&mutex, 0, 0);

	pthread_create(&ta, 0, &ta_loop, NULL);

	for (k = 0; k < NUM_OF_STUDENTS; k++) {
		pthread_create(&students[k], 0, &student_loop, (void *)&student_id[k]);
	}

        for (i = 0; i < NUM_OF_STUDENTS; i++){
                pthread_join(students[i], NULL);
	}

	pthread_join(ta, NULL);

	return 0;
}



void *student_loop(void *param)
{
	int number = *((int *)param);

	int value;
	int temp = 0;
	printf("Student %i hanging out for 5 second\n", number);
	sleep(MAX_SLEEP_TIME);

	while(1){
		if(sem_wait(&onChair) ==3){
			printf("Student %i will try later\n", number);
		}else{
			if(sem_wait(&taSleep) == 0){
				temp = number;
				sem_wait(&taSleep);
				sem_wait(&tAseat);
				printf("Student %i Receiving help", number);
				sem_post(&tAseat);
				printf("Student %i Hanging out for %d seconds ", number,rnWait);


			} else{
				// take a seat outside
				sem_wait(&onChair);
				printf("Student % i takes a seat, waiting =  % i \n",number,temp);

				//wait on ta's office chair to become free
				sem_wait(&tAseat);
				// empty the chair outside
				sem_post(&onChair);

				//Student with the ta
				printf("Student %i Receiving help\n", number);

				//Give up the ta's room chair
				sem_post(&tAseat);
				printf("Student %i Hanging out for %i seconds \n", number,rnWait);
			}
		}

	}
	return NULL;

}
void *ta_loop(){
    int value;
    while (sem_wait(&onChair) !=0){
	   //take a random time to help student
	    printf("Helping a Student for %d seconds, waiting  = %i \n",time,sem_getvalue(&onChair, &value)  );

	    rnWait();

	    // Release the student after tutoring
	    sem_post(&tAseat);
    }
}

void rnWait()
{
   int len;
   srand((unsigned)time(NULL));
    // Generate a random number...
    len = (rand() % NUM_OF_STUDENTS );
    sleep(len);
}

