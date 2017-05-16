#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	if (argc > 2)
	{
		printf("only one arguments needed \n");
		exit(EXIT_FAILURE);
	}else if(argc < 2)
	{
		printf("atleast one argument expected \n");
		exit(EXIT_FAILURE);		
	}
	int num  = atoi(argv[1]);
	pid_t pid;
	pid = fork();
	if(pid ==0)
	{
		printf("%i, ",num);
		while(num  != 1)
		{
			if(num % 2)
			{
				num =  3 * num + 1;
			}else{
		 		num =  num/2;
			}
			printf("%i, ",num);
		}
		printf("\n");

	}else{
	wait();
	printf("The program is completed \n");
	return 0;
	}
	
}