#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void print_m(int * m, int n, int v)
{	
	int i, j;
	for (i = 0; i < n; i++){
			for (j = 0; j < v; j++){
				printf("%d\t", *(m + i * v + j)); 
			}
		putchar('\n');
	}
	putchar('\n');
	putchar('\n');
}

int calc_med(int * me, int me_s)
{
	float media;
	int i;
	for (i = 0; i < me_s; i++){
		media = (float) (me[i] + media)/ 2;
	}
	return (int) roundf(media);
}

int main (int argc, char ** argv)
{
	int n, v, i, j, m_s, pid;
	int * m;
	/* Check input */
	if (argc < 3){
		fprintf(stderr, "%s N V\n", argv[0]);
		exit(-1);
	}
	n = atoi(argv[1]);
	v = atoi(argv[2]);

	/* Create and get vector */
	m_s = n * v;
	m = (int *) malloc(sizeof(int) * m_s);
	for (i = 0; i < n; i++)
			for (j = 0; j < v; j++)
				scanf("%d", m + i * v + j); 

	for (i = 0; i < n; i++) {
		pid = fork();
		if (pid == 0){
			exit(calc_med(m + i * v, n));
		} else if (pid > 0) {
			int status;
			pid = wait(&status);
			printf("pid: %d\tstatus: %d\tindex: %d\n", pid, WEXITSTATUS(status), i);
		} else {
			fprintf(stderr, "Something wrong 2\n");
			exit(-1);
		}		
	}
}
