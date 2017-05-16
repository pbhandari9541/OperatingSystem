// pramesh bhandari
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1
int main(int argc, char *argv[])
{	
	
	if (argc > 3)
	{
		printf("only input and output file name needed \n");
		return 1;
	}else if(argc  != 3)
	{
		printf("input and outputfile expected \n");
		return 1;		
	}

	char write_msg[BUFFER_SIZE]= "";
	char read_msg[BUFFER_SIZE];	
	int fd[2];
	pid_t pid;
	int start = 0;
	int pipe_return = pipe(fd);
	FILE* finput;
	FILE* foutput;
	char temp;
       	char str[BUFFER_SIZE] = "";

	finput = fopen(argv[1],"r");
	foutput = fopen(argv[2], "w+");

	printf("Reading a file %s \n", argv[1]);
	printf("Writing a file %s \n", argv[2]);
	if(finput == NULL){
		printf("File doesn't exist\n");
		return 1;
	}


	while(fscanf(finput, "%s",str) != EOF){
		
		strcat(str, "");
		strcat(write_msg, str);		
	}

	int end = strlen(write_msg) - 1;

	while(start < end) 
	{		
		temp = write_msg[start];
		write_msg[start] = write_msg[end];
		write_msg[end]	= temp;
		start++;
		end--;
	}

	fclose(finput);
	if(pipe_return ==-1)
	{
		printf("unable to create pipe\n");
		exit(EXIT_FAILURE);	
	}

	write(fd[WRITE_END], write_msg, strlen(write_msg)+1); 
	/* close the write end of the pipe */
	close(fd[WRITE_END]);
	/* now fork a child process */
	pid = fork();	
	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if (pid > 0) {  /* parent process */
		wait(pid);		
		return 0;

	}
	if(pid == 0){ /* child process */
		/* close the unused end of the pipe */
		close(fd[WRITE_END]);
		/* read from the pipe */
		read(fd[READ_END], read_msg, BUFFER_SIZE);

		fprintf(foutput, "%s", read_msg);
		printf("file contents has already been reversed. please check the output file\n");
		fclose(foutput);

		/* close the write end of the pipe */
		close(fd[READ_END]);
	}
}

	
	