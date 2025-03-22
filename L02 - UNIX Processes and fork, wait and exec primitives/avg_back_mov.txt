
#include <stdio.h>
#include <stdlib.h>
#define MAXN 30
#define MAXV 10

int M[MAXN][MAXV];
int V;
int N;

void figlio(int indice);
int cerca_indice(int *pid, int p);

void main(int argc, char *argv[]) {
int i, j, k, status, n_children;
int pid[MAXN], p; 


if (argc!=3)
{   printf("%s: sintassi sbagliata\n", argv[0]);
    exit(1);
}

N = atoi(argv[1]);
V = atoi(argv[2]);


for ( i=0; i<N; i++ ) {
    printf("immetti i %d voti dello studente %d:\n", V, i);
    for(j=0; j<V; j++)
       scanf("%d", &M[i][j]);
}

// stampa matrice:
printf("\n ----- DATI -----\n");
for ( i=0; i<N; i++ ) {
    printf("voti dello studente %d:",  i);
    for(j=0; j<V; j++)
    {   
        printf("\t%d", M[i][j]);
    }
    printf("\n");
}
printf("----------------\n\n");


for ( i=0; i<N; i++ ) { // creazione figli
    pid[i] = fork();
    if ( pid[i] == 0 ) { // Eseguito dai figli
        figlio(i);
    }
    else if ( pid[i] > 0 ) { // Eseguito dal padre
        printf("%d: ho creato il figlio %d (PID %d)\n", getpid(),i, pid[i]);
    }
    else {
        perror("Fork error:");
        exit(1);
    }
    

}

for ( i=0; i<N; i++ ) { // attesa figli
    p=wait(&status);
    k=cerca_indice(pid,p); // ricerca indice del figlio di pid p
    if ((char)status==0) 
        printf("\nIl voto dello studente n.%d (pid=%d) Ã¨ %d\n",k,p, status>>8);
    else 
        printf("term. involontaria  per segnale %d\n",(char)status);
}



}

void figlio(int indice)
{   int voto, j;
    float media=0;

    for(j=0;j<V; j++)
        media+=M[indice][j];
    media=media/V; // valore reale
    voto=(int)media; // parte intera
    if (media>=(voto+0.5)) // arrotondamento verso l'alto
        voto++;
    //printf("studente %d - media: %f \t voto: %d\n", indice, media, voto);
    exit(voto);
}

int cerca_indice(int *pid, int p){
    int i;
    for(i=0;i<N; i++)
        if (pid[i]==p)
            return i;
}
-----------------------------------------------------------------------------------------------
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_STRING_LENGTH 255
#define MAXP 20

int num_files;
int cerca_indice(int *pid, int p);
void usage(char* prog_name);

int main (int argc, char* argv[]){
    int i, k;
    int pid[MAXP];
    
    char buff[MAX_STRING_LENGTH];/* Buffer temporaneo dove, per comoditÃ  scrivo i messaggi di errore prima di stamparli */
    char dirdest[MAX_STRING_LENGTH];
    char suffisso[]=".bak";
    
    if ( argc <= 1 ) {/*Almeno un file da compilare*/
        fprintf(stderr,"Numero di argomenti non valido\n");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    strcpy(dirdest, argv[argc-1]);
    printf("directory di destinazione: %s\n", dirdest);
    
    num_files = argc - 2;

    for( i=0; i<num_files; i++ ){
        pid[i] = fork();
        if ( pid[i] > 0 ) { /* Processo padre*/
            printf("Padre %d: creato figlio %d con pid: %d\n", getpid(), i, pid[i]);
        }
        else if ( pid[i] == 0 ) { /*Processo figlio adibito alla copia */
            
            // "Costruisco" il nome della copia
            char newfile[MAX_STRING_LENGTH]; 
            strcpy(newfile, dirdest);
            strcat(newfile, "/");
            strcat(newfile, argv[i+1]);
            strcat(newfile, suffisso);
            printf("Figlio %d: copio il file %s nel file %s \n", i, argv[i+1], newfile);
            execlp("cp", "cp", argv[i+1], newfile, (char*)0 );
            sprintf(buff, "Figlio %d: impossibile copiare il file %s", i, argv[i+1]);
            perror(buff);
            exit(EXIT_FAILURE); 
        }else {
            sprintf(buff,"Padre: impossibile fare fork %d",i);
            perror(buff);
            exit(EXIT_FAILURE);
        }
    }

    
    for ( i=0; i<num_files; i++ ){
        int status, terminated_pid;
        terminated_pid = wait(&status);
        k=cerca_indice(pid, terminated_pid);
        if (WIFEXITED(status)) 
        {    if ( ! (WEXITSTATUS(status) == EXIT_SUCCESS) ){  
            fprintf(stderr,"Padre: il figlio con PID=%d  Ã¨ fallito.\n", terminated_pid);
            }
        }
        else {
            printf("term. involontaria del figlio (pid= %d, file= %s per segnale %d\n",terminated_pid, argv[k+1], (char)status);
}
        
    }
    printf("\nPadre: ecco il contenuto della directory %s: \n", dirdest);
    execlp("ls", "ls", "-l", dirdest, (char*)0);
    perror("ls del padre fallita -");
    return EXIT_FAILURE; // se il padre arriva qui, l'execlp Ã¨ fallita
}


int cerca_indice(int *pid, int p){
    int i;
    for(i=0;i<num_files; i++)
        if (pid[i]==p)
            return i;
}

void usage(char* prog_name){
    fprintf(stderr,"Usage:\n\t%s file1 [file2 ... ] dirdest\n", prog_name);
}
---------------------------------------------------------------------------------------------------
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_STRING_LENGTH 255
#define MAXP 20

int num_files;
void usage(char* prog_name);

int main (int argc, char* argv[]){
    int i, k;
    int pid[MAXP], nipote, terminated_pid, status;
    
    char buff[MAX_STRING_LENGTH];/* Buffer temporaneo dove, per comoditÃ  scrivo i messaggi di errore prima di stamparli */
    char dirdest[MAX_STRING_LENGTH];
    char suffisso[]=".bak";
    
    if ( argc <= 1 ) {/*Almeno un file da compilare*/
        fprintf(stderr,"Numero di argomenti non valido\n");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    strcpy(dirdest, argv[argc-1]);
    printf("directory di destinazione: %s\n", dirdest);
    
    num_files = argc - 2;

    for( i=0; i<num_files; i++ ){
        pid[i] = fork();
        if ( pid[i] > 0 ) { /* Processo padre*/
            printf("Padre %d: creato figlio %d con pid: %d\n", getpid(), i, pid[i]);
        }
        else if ( pid[i] == 0 ) { /*Processo figlio adibito alla cancellazione */
            
            printf("Figlio %d: creo il processo copiatore dei file %s \n", i, argv[i+1]);
            nipote=fork(); // creazione NIPOTE copiatore
            if (nipote==0) //codice copiatore
            {   printf("nipote %d (pid=%d) sto per copiare il file %s\n", i,getpid(), argv[i+1]);
                execlp("cp", "cp", argv[i+1], dirdest, (char*)0 );
                sprintf(buff, "nipote %d: impossibile copiare il file %s", i, argv[i+1]);
                perror(buff);
                exit(EXIT_FAILURE); 
            }
            else if (nipote>0) //CODICE FIGLIO CANCELLATORE
            {   terminated_pid=wait(&status);
                if (WIFEXITED(status)) { // terminazione volontaria nipote copiatore          
                    if ( ! (WEXITSTATUS(status) == EXIT_SUCCESS) ){  // con fallimento
                        fprintf(stderr,"%d: il nipote copiatore con PID=%d  Ã¨ fallito.\n", terminated_pid);
                        exit(EXIT_FAILURE); // figlio termina per fallimento nipote
                    }
                }   
            else { // terminazione involontaria nipote copiatore 
                printf("term. involontaria del figlio (pid= %d) per segnale %d\n",terminated_pid, (char)status);
                exit(EXIT_FAILURE); // figlio termina per term involontaria nipote
            }
            // il nipote ha concluso volontariamente con successo -> figlio: cancellazione
            execlp("rm","rm", argv[i+1], (char*)0);
            printf(buff, "figlio %d: impossibile cancellare il file %s", i, argv[i+1]);
            perror(buff);
            exit(EXIT_FAILURE); 
            
            }// fine nipote
    }
    }
    
    for ( i=0; i<num_files; i++ ){ //padre: attende tutti i figli
        terminated_pid = wait(&status); // attendo la terminazione dei figli
        if (WIFEXITED(status)) 
        {    if ( ! (WEXITSTATUS(status) == EXIT_SUCCESS) ){  
            fprintf(stderr,"Padre: il figlio con PID=%d  Ã¨ fallito.\n", terminated_pid);
            }
        }
        else {
            printf("term. involontaria del figlio (pid= %d, file= %s per segnale %d\n",terminated_pid, argv[k+1], (char)status);
        }
    }
   
    return EXIT_SUCCESS;
}

void usage(char* prog_name){
    fprintf(stderr,"Usage:\n\t%s file1 [file2 ... ] dirdest\n", prog_name);
}
------------------------------------------------------------------------------------------------
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
----------------------------------------------------------------------------------------------------
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
----------------------------------------------------------------------------------------------------
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
