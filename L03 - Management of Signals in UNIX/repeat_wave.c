#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/wait.h>

void child_handler()
{
	kill(getppid(), SIGUSR1);
	exit(EXIT_SUCCESS);
}


int main(int argc, char** argv)
{
	int pid, n;

	//args checking to be done
	n = atoi(argv[1]);
	
	pid = fork();
	if (pid == 0){
		char buf[10];
		int i, ignore;
		signal(SIGALRM, child_handler);
		alarm(n);
		for (i = 0; true; i++){
			sprintf(buf, "%d", i);
			pid = fork();
			if (pid > 0) {
				execl("./hello", "hello", buf, (char *) NULL);
				exit(EXIT_SUCCESS);
			} else if (pid < 0) {
				puts("Something went wrong\n");
				exit(-2);
			} else {
				wait(&ignore);
			}
		}
	} else if (pid > 0){
		pause();
		printf("Smthing\n"); // right printing to be done
		exit(EXIT_SUCCESS);
	} else {
		puts("Something went wrong\n");
		exit(-1);
	}

}
