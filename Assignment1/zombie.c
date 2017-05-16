#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
	pid_t pid;
	pid = fork();

	if (pid == 0)
	{	
		printf("I am child \n");
	}else if (pid > 0){
		printf("zomnbie created");
		sleep(10);
	}
	
	return 0;


}