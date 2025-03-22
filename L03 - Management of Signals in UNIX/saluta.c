#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char** argv)
{
	int pid, n;

	//args checking to be done
	n = atoi(argv[1]);
	
	pid = fork();
	if (pid == 0){
		int i;
		for (i = 0; i < n; i++){
			printf("Hello World %d\n", i + 1);
			sleep(1);
		}
		kill(getppid(), SIGUSR1);
		exit(EXIT_SUCCESS);
	} else if (pid > 0){
		pause();
		printf("Smthing\n"); // right printing to be done
		exit(EXIT_SUCCESS);
	} else {
		puts("Something went wrong\n");
		exit(-1);
	}

}
