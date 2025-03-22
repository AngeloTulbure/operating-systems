#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char** argv)
{
	int n_file, pid_cp, pid_rm, i;
	char * dest_dir;
	
	if (argc < 3){
		fprintf(stderr, "%s file1 file2 ... fileN dest_dir\n", argv[0]);
		exit(-1);
	}
	
	dest_dir = argv[argc - 1];
	for (i = 1; i < argc - 1; i++){
		pid_rm = fork();
		if (pid_rm == 0){
			execl("/usr/bin/rm", "rm", argv[i], NULL);
			exit(EXIT_SUCCESS);
		} else if (pid_rm > 0) {
			pid_cp = fork();
			if (pid_cp == 0){
				char * new_name;	
				new_name = (char *) malloc(strlen(argv[i]) + strlen(dest_dir) + 8); // dotslash + slash + .bak + '\0'
				sprintf(new_name, "./%s/%s.bak", dest_dir, argv[i]);
				execl("/usr/bin/cp", "cp", argv[i], new_name, NULL);
				free(new_name);
				kill(pid_rm, SIGUSR1);
				exit(EXIT_SUCCESS);
			} else if (pid_cp > 0) {
				int status_cp;
				pid_cp = wait(&status_cp);
				if (!WIFEXITED(status_cp))
					printf("pid: %d\t nome_file: %s\n", pid_cp, argv[i]);
			} else {
				fprintf(stderr, "Smthing wrong 2\n");
				exit(-1);
			}
			int status_rm;
			pid_rm = wait(&status_rm);
			if (!WIFEXITED(status_rm))
				printf("pid: %d\t nome_file: %s\n", pid_rm, argv[i]);
		} else {
			fprintf(stderr, "Smthing wrong 3\n");
			exit(-1);
		}
	}
	
	execl("/usr/bin/ls", "ls", dest_dir, NULL);
}
