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
                        fprintf(stderr,"%d: il nipote copiatore con PID=%d  è fallito.\n", terminated_pid);
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
            fprintf(stderr,"Padre: il figlio con PID=%d  è fallito.\n", terminated_pid);
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
