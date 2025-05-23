#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void child(char * fileout)
{
	int fileno;
	close(STDOUT_FILENO);
	fileno = open(fileout, O_CREAT | O_WRONLY, 0666);
	/* da sostituire con il primo esercizio */
	execl("/usr/bin/echo", "echo", "bbb\nabh\nhbf\ndsadsa\nsadm", NULL);
	sleep(1000);
}


int main(int argc, char ** argv)
{
	char * fileout_s;
	char * timeout_s;
	int timeout, pid, ignored;
	char * fase_s;

	if (argc != 4){
		fprintf(stderr, "Err: wrong argc\n");
		exit(-1);
	}


	fileout_s = argv[1];
	timeout_s = argv[2];
	fase_s = argv[3];

	timeout = (int) strtol(timeout_s, NULL, 10);

	pid = fork();
	if (pid == 0) {
		child(fileout_s);
	} else {
		sleep(timeout);
		kill(pid, 9); //non mi ricordo la macro corrispondente
		execl("/usr/bin/grep", "grep", fase_s, fileout_s, NULL);
	}

}
