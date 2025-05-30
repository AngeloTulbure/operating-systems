#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

// COD-mm;ss

#define BUF_SIZE 9
void read_and_send(int in, int pipe)
{
	char buf[BUF_SIZE];
	lseek(in, 0, SEEK_END);
	do {
		read(in, buf, BUF_SIZE);
		buf[BUF_SIZE - 2] = '\0';
		write(pipe, buf, BUF_SIZE);
	} while (lseek(in, -BUF_SIZE * 2, SEEK_CUR) >= 0);
}

void print_from_pipe(int pipe)
{
	char buf[BUF_SIZE];
	while (read(pipe, buf, BUF_SIZE)){
		puts(buf);
	}
}
#undef BUF_SIZE

int main(int argc, char ** argv)
{
	int pipefd[2], pid;
	pipe(pipefd);
	pid = fork();
	if (pid == 0){
		int classfd;
		close(pipefd[0]);
		classfd = open(argv[1], O_RDONLY);
		read_and_send(classfd, pipefd[1]);
		close(pipefd[1]);
	} else {
		int ignored;
		close(pipefd[1]);
		print_from_pipe(pipefd[0]);
		wait(&ignored);
	}
}
