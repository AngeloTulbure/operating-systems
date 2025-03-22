#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void reverse_file_lines(int in, int out)
{
	#define BUF_SIZE 10
	off_t pos;
	char buf[BUF_SIZE];
	lseek(in, -BUF_SIZE, SEEK_END);
	do {
		read(in, buf, BUF_SIZE);
		write(out, buf, BUF_SIZE);
		write(STDOUT_FILENO, buf, BUF_SIZE);
	} while (lseek(in, -BUF_SIZE * 2, SEEK_CUR) >= 0);
	#undef BUF_SIZE
}

int main(int argc, char ** argv)
{
	const char * class_name = argv[1];
	int class, speaker, pid, ignored;
	class = open(class_name, O_RDONLY);
	speaker = open("./SPEAKER", O_WRONLY| O_CREAT| O_TRUNC, 00666);
	pid = fork();
	if (pid == 0){
		reverse_file_lines(class, speaker);	
	} else {
		wait(&ignored);
	}
}
