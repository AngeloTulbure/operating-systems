#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>

void print_from_pipe(int pipe)
{
	int tmp;
	while (read(pipe, &tmp, sizeof(int))){
		printf("Il massimo Ã¨: %d\n", tmp);
	}
}


#define BUF_SIZE 1024
int calc_max_from_pipe(int pipe)
{
	int buf[BUF_SIZE], num_ints = 0, i, out = INT_MIN;
	/* The code below might fail*/
	while (num_ints = read(pipe, buf, sizeof(int) * BUF_SIZE) / sizeof(int)){
		for (i = 0; i < num_ints; i++){
			if(out < buf[i])
				out = buf[i];
		}
	}
	return out;
}

#undef BUF_SIZE
#define BUF_SIZE 2
void read_file_and_send(int fd, int pipe)
{
	int buf[BUF_SIZE], tosend; //not necessary, more readable
	while (read(fd, buf, BUF_SIZE * sizeof(int))){
		if (buf[0] > buf[1]){
			tosend = buf[0];
		} else {
			tosend = buf[1];
		}
		write(pipe, &tosend, sizeof(int));
	}
}
#undef BUF_SIZE

int main(int argc, char ** argv)
{
	int pipe1[2], pid1, filenum, i;
	char * fin_str;
	fin_str = argv[1];

	pipe(pipe1);
	pid1 = fork();
	if (pid1 == 0){ 
		int pid2, pipe2[2];
		close(pipe1[0]);
		pipe(pipe2);
		pid2 = fork();
		if (pid2 == 0){ //P2
			int fd;
			close(pipe2[0]);
			fd = open(fin_str, O_RDONLY);
			read_file_and_send(fd, pipe2[1]);
			close(pipe2[1]);
			exit(EXIT_SUCCESS);
		} else { // P1
			int ignored, tmp;
			close(pipe2[1]);
			tmp = calc_max_from_pipe(pipe2[0]);
			write(pipe1[1], &tmp, sizeof(int));
			close(pipe1[1]);
			wait(&ignored);
			exit(EXIT_SUCCESS);
		}
	} else {
		int ignored;
		close(pipe1[1]);
		print_from_pipe(pipe1[0]);
		wait(&ignored);
	}
}
