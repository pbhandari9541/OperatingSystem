#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define COMMAND_TOK_BUFSIZE 30

double avg;//the running sum will be saved here
int sum;
int min;//global variable
int max;
int count;
void* add(void*);
void* minimum(void* arg);
void* maximum(void* arg);
int main(int argc, char *argv[])
{
	int i;

	char *args1[COMMAND_TOK_BUFSIZE];
	count = argc;
	//error-correction()
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <positive integer \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++){
		args1[i - 1] = argv[i];
		printf("args[%i]:%s\n", i, args1[i - 1]);
	}

	pthread_t thread1id;
	pthread_t thread2id;
	pthread_t thread3id;

	pthread_attr_t thread1attr;
	pthread_attr_t thread2attr;
	pthread_attr_t thread3attr;

	pthread_attr_init(&thread1attr);//initialize attributes to default
	pthread_attr_init(&thread2attr);
	pthread_attr_init(&thread3attr);

	pthread_create(&thread1id, &thread1attr, &add, (void *)argv);// create thead 1
	pthread_create(&thread2id, &thread2attr, &minimum, (void *)argv);// create thead 2
	pthread_create(&thread3id, &thread3attr, &maximum, (void *)argv);// create thead 2

	pthread_join(thread1id, NULL);
	pthread_join(thread2id, NULL);
	pthread_join(thread2id, NULL);
	printf("The sum is: %f\n the min is: %i \n the max is : %i \n", avg, min, max);


}
void* add(void* arg)
{
	sum = 0;
	int j;
	char **args = (char **)arg;

	for (j = 1; j< count; j++)
	{
		//printf("arg[%i]:%i\n",j,atoi(arg[j+1]));
		sum = sum + atoi(args[j]);
	}
	avg = (sum / (count - 1));
	pthread_exit(EXIT_SUCCESS);
}
void* minimum(void* arg)
{
	int k = 0;
	char **args = (char **)arg;
	min = 1000;
	for (k = 1; k< count; k++){
		if (min > atoi(args[k]))
		{
			min = atoi(args[k]);
		}
	}
	pthread_exit(EXIT_SUCCESS);
}
void* maximum(void* arg)
{
	int l;
	char **args = (char **)arg;
	max = 0;
	for (l = 1; l< count; l++){
		if (max < atoi(args[l]))
		{
			max = atoi(args[l]);
		}
	}
	pthread_exit(EXIT_SUCCESS);
}

