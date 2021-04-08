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
			printf("Couldn't discern input: it's not ls and not cat!\n");
		}
		
		v_args[i] = NULL;

		pid = fork();
		switch (pid)
		{
		case -1: //if error
		{
			printf("Fork failed!\n");
			exit(-1);
		}
		case 0: //if child process
		{
			execvp(v_args[0], v_args);
			break;
		}
		default: //if parent process
		{
			(void)signal(SIGINT, onCtrlC);
			int stat;
			wait(&stat);
			printf("Child process successfully ended with code %d.\n", WEXITSTATUS(stat));
			break;
		}
		}
	}
	return 0;
}

void onCtrlC(int sig) {
	printf("Got signal to kill the child %d\n", sig);
	if(kill(pid, SIGINT) == 0) 
		printf("Child killed!\n");
	else
	{
		printf("Some error has occured while killing the child!\n");
		return;
	}
}