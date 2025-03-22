#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
	

#define BUF_SIZE 8
void reverse_file_lines(int in, int out)
{
	char buf[BUF_SIZE];
	lseek(in, 0, SEEK_END);
	do {
		read(in, buf, BUF_SIZE);
		write(out, buf, BUF_SIZE);
	} while (lseek(in, -BUF_SIZE * 2, SEEK_CUR) >= 0);
}


void print_vinc(int inv, int prem, int n)
{
	char buf1[BUF_SIZE-3];
	char buf2[3];
	char tmp_char;
	int tmp_i;
	int i;
	for (i = 1; i <= n; i++) {
		read(inv, buf1, BUF_SIZE-3);
		read(inv, buf2, 3);
		read(prem, &tmp_i, sizeof(int));
		printf("Il sig. %s ha vinto il premio n.%d (euro %d) con il punteggio %s", buf1, i, tmp_i, buf2);
	}
}
#undef BUF_SIZE

int main(int argc, char ** argv)
{
	char * punt_name, * premi_name;
	int punt, premi, fileINV, vincitori, pid1, pid2, ignored;

	punt_name = argv[1];
	premi_name = argv[2];

	punt = open(punt_name, O_RDONLY);
	premi = open(premi_name, O_RDONLY);
	fileINV = open("fileINV", O_WRONLY| O_CREAT| O_TRUNC, 00666);
	vincitori = open("VINCITORI", O_WRONLY| O_CREAT| O_TRUNC, 00666);
	
	pid1 = fork();
	if (pid1 == 0){
		pid2 = fork();
	} else {
		wait(&ignored);
	}
}
