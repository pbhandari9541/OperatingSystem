#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
int main(int argc, char **argv)
{
	if (argc > 2)
	{
		printf("only one arguments needed \n");
		exit(EXIT_FAILURE);
	}else if(argc != 2)
	{
		printf("atleast one argument expected \n");
		exit(EXIT_FAILURE);		
	}
	const int SIZE  = 4096;
	const char *name = "OS";
	int shm_fd;
	void *ptr;
	char string1[5] = "";
	char string2[100] = "";
	
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);

	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	int num  = atoi(argv[1]);
        sprintf(string2, "%d \n", num);
	pid_t pid;
	pid = fork();
	if(pid ==0)
	{
		while(num  != 1)
		{
			if(num % 2)
			{
				num =  3 * num + 1;
			}else{
		 		num =  num/2;
			}
			sprintf(string1, "%d \n", num);
			strcat(string2, string1);
		}
		
		sprintf(ptr, "%s \n", string2);
		ptr += strlen(string2);

	}else{
	wait();
	//shm_fd = shm_open(name, O_RDONLY, 0666);
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	printf("%s", ptr);
	shm_unlink(name);
	printf("The program is completed \n");
	return 0;
	}
	
}