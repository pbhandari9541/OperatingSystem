#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
int count = 0;
float pi;
struct point{
	float x;
	float y;
};
int ptNo = 0;
struct point pt[10000000];
void rn();
int main(int argc, char **argv){
	//error-correction()

	if (argc != 2 || argc >2)
	{
		fprintf(stderr, "Usage: %s <positive integer \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	ptNo = atoi(argv[1]);

#pragma omp parallel
	{
		rn();
	}

	printf("the value of pi: % .4f\n", pi);
}
void rn(){
	int j = 0;


	float distance;
	srand(time(NULL));
	while (j< ptNo){
		pt[j].x = ((((float)rand() / RAND_MAX) * 2) - 1);
		pt[j].y = ((((float)rand() / RAND_MAX) * 2) - 1);
		distance = sqrtf((pt[j].x * pt[j].x) + (pt[j].y * pt[j].y));
		//printf("point[%i]: (%.2f, %.2f)\n", j,pt[j].x, pt[j].y);
		if (distance <= 1){

			count++;
		}
		j++;
	}

	pi = 4 * (float)count / (float)ptNo;
}

