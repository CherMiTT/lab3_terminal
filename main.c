#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void onCtrlC(int sig);

pid_t pid;


int main(int argc, char *argv[])
{
	printf("Started programm\n");

	char input[1024];
	char* ptr = NULL;
	char* v_args[32];
	char tmp[32][128];

	while(1)
	{
		if(fgets(input,1023, stdin) == NULL)
		{
			printf("Some error has occured\n");
			return 0;
		}
		input[1023] = '\0';

		if(!strncmp("exit", input, 4)) //if first 4 characters is "exit"; all other words are irrelevant
		{
			printf("Exiting application\n");
			return 0;
		}

		ptr = strtok(input, " \n\0");
		strcpy(tmp[0], ptr);
		v_args[0] = tmp[0];

		printf("First word = %s\n", tmp[0]);

		int i = 1;
		while ((ptr = strtok(NULL, " \n")))
		{
			strcpy(tmp[i], ptr);
			v_args[i] = tmp[i];
			i++;
		}

		if(!strcmp("cat", tmp[0]))
		{
			printf("Processing cat command\n");
		}
		else if(!strcmp("ls", tmp[0]))
		{
			printf("Processing ls command\n");
		}
		else
		{
			printf("Couldn't discern input\n");
		}
		
		v_args[i] = NULL;

		pid = fork();
		if(pid == 0) //if child process
		{
			//(void)signal(SIGINT, SIG_IGN);
			execvp(v_args[0], v_args);
		}
		else
		{
			(void)signal(SIGINT, onCtrlC);
			int stat;
			wait(&stat);
			printf("Parent process ended.\n");
		}
	}
	return 0;
}

void onCtrlC(int sig) {
	//printf("OUCH! - I got signal %d\n", sig);
	//(void)signal(SIGINT, SIG_DFL);
	printf("Got signal to kill the child %d\n", sig);
	kill(pid, SIGINT);
	printf("Child killed!\n");
}