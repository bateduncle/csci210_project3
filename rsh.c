#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

char *argv[] = {"echo", "Hello from the spawned process!", NULL};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for(int i = 0; i < N; i++)
	{
		if(strcmp(cmd, allowed[i]) == 0)
		{	
			return 1;
		}
	}
	

	return 0;
}

int main() {

    // TODO
    // Add variables as needed
	pid_t pid;
	int status;



	char line[256];
	char copyOfLine[256];
	char* args[20];
	int argc;
	char* token;

    while (1) {
	
	argc = 0;

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';
	
	strcpy(copyOfLine, line);
	
	token = strtok(line, " ");

	while(token != NULL)
	{
		args[argc] = token;
		argc++;
		token = strtok(NULL, " ");
	}	
	args[argc] = NULL;

	if(isAllowed(args[0]) == 0)
	{
		printf("NOT ALLOWED!\n"); 
	}
	
	else
	{


	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c
	

		if(strcmp(args[0], "exit") == 0)
		{
			return 0;
		}
		else if(strcmp(args[0], "cd") == 0)
		{
			if(argc > 2)
			{
				printf("-rsh: cd: too many arguments\n");
			}
			else{
				chdir(args[1]);
			}
		}

		else if(strcmp(args[0], "help") == 0)
		{
			printf("The allowed commands are:\n");

			for(int i = 0; i < N; i++)
			{
				printf("%d: %s\n", i + 1, allowed[i]);
			}
		}

		else{

			posix_spawnattr_t attr;

			posix_spawnattr_init(&attr);

    // Set flags if needed, for example, to specify the scheduling policy
    // posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);

    // Spawn a new process
    			if (posix_spawnp(&pid, args[0], NULL, &attr, args, environ) != 0) {
        			perror("spawn failed");
        			exit(EXIT_FAILURE);
    			}

    // Wait for the spawned process to terminate

			if (waitpid(pid, &status, 0) == -1) {
        			perror("waitpid failed");
        			exit(EXIT_FAILURE);
    			}	 


			posix_spawnattr_destroy(&attr);
		}
	}
   }
    return 0;
}
