#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>

#define BUF_SIZE 1024
void print_from_pipe(char * file, char * parola, int pipe)
{
	char buf[BUF_SIZE];
	int num_chars;
	while (num_chars = read(pipe, buf, BUF_SIZE)){
		buf[num_chars - 1] = '\0'; //remove newline
		printf("nel file %s sono state trovate %s occurrenze di %s\n", file, buf, parola);
	}
}
#undef BUF_SIZE

void grepper(char * file, char * parola, int pipe)
{
	dup2(pipe, STDOUT_FILENO);
	close(pipe);
	execl("/bin/grep", "grep", "-c", parola, file, NULL);
}

int main(int argc, char ** argv)
{
	int *pipev, pid, filenum, i;
	char * parola;
	filenum = argc - 2;
	parola = argv[argc - 1];
	pipev = malloc(sizeof(int) * 2 * filenum);
	for (i = 0; i < filenum; i++){
		pipe(pipev + i);
		pid = fork();
		if (pid == 0){
			int classfd;
			close(pipev[i + 0]);
			grepper(argv[i + 1], parola, pipev[i + 1]);
			close(pipev[i + 1]);
			exit(EXIT_SUCCESS);
		} else {
			int ignored;
			close(pipev[i + 1]);
			print_from_pipe(argv[i + 1], parola, pipev[i + 0]);
			wait(&ignored);
		}
	}
}
