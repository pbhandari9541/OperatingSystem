#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
char array [1000];
int ptNo = 0;
void fibonnaci();
int main(int argc, char **argv){

	int i = 0;
	if (argc != 2 || argc >2)
	{
		fprintf(stderr, "Usage: %s <positive integer \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	ptNo = atoi(argv[1]);
	//parallel
	#pragma omp parallel
	{
		fibonnaci();
	}
	for(i = 0; i < ptNo; i++){
		printf("the value of pi: % i\n", array[i]);
	}
}
void fibonnaci(){
	int j = 0;
	array[0] = 0;
	array[1] = 1;
	for(j = 2; j < ptNo; j++){
		array[j] = array[j-1] + array[j-2];

	}
}

