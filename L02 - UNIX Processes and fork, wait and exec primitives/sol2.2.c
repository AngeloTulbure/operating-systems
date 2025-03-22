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
    
    char buff[MAX_STRING_LENGTH];/* Buffer temporaneo dove, per comodità scrivo i messaggi di errore prima di stamparli */
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
            fprintf(stderr,"Padre: il figlio con PID=%d  è fallito.\n", terminated_pid);
            }
        }
        else {
            printf("term. involontaria del figlio (pid= %d, file= %s per segnale %d\n",terminated_pid, argv[k+1], (char)status);
}
        
    }
    printf("\nPadre: ecco il contenuto della directory %s: \n", dirdest);
    execlp("ls", "ls", "-l", dirdest, (char*)0);
    perror("ls del padre fallita -");
    return EXIT_FAILURE; // se il padre arriva qui, l'execlp è fallita
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
