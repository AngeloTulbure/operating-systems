#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
	int n_file, pid, i;
	char * dest_dir;
	
	if (argc < 3){
		fprintf(stderr, "%s file1 file2 ... fileN dest_dir\n", argv[0]);
		exit(-1);
	}
	
	dest_dir = argv[argc - 1];
	for (i = 1; i < argc - 1; i++){
		pid = fork();
		if (pid == 0){
			char * new_name;
			new_name = (char *) malloc(strlen(argv[i]) + strlen(dest_dir) + 8); // dotslash + slash + .bak + '\0'
			fprintf(new_name, "./%s/%s.bak", dest_dir, argv[i]);
			execl("/usr/bin/cp", "cp", argv[i], new_name, NULL);
			free(new_name);
			exit(EXIT_SUCCESS);
		} else if (pid > 0) {
			int status;
			pid = wait(&status);
			if (!WIFEXITED(status))
				printf("pid: %d\t nome_file: %s\n", pid);
		} else {
			fprintf(stderr, "Smthing wrong 2\n");
			exit(-1);
		}
	}
	
	execl("/usr/bin/ls", "ls", dest_dir, NULL);
}
