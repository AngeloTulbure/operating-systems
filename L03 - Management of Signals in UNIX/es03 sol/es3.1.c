#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>


void print_usage(char* prog_name){
    fprintf(stderr, "Usage:\n\t%s N\n", prog_name);
}

void alarm_handler(int signo){
    printf("P1 (pid=%d): timeout exceeded.\n", getpid());
    kill(getppid(),SIGUSR1);
    exit(EXIT_SUCCESS);
}

void father_handler(int signo){
    printf("P0 (pid=%d): ricevuto segnale da P1.\n", getpid());
}


void wait_child(){
    int status, pid;
    pid = wait(&status);
    printf("Terminato processo figlio PID=%d : ", pid);
        if ((char)status==0) 
            printf("term. volontaria con stato %d\n", status>>8);
        else 
            printf("term. involontaria per segnale %d\n", (char)status);
}


int main(int argc, char* argv[])
{
    int N, count, pid;

    /*
     * Controllo argomenti    
     */
    if (argc != 2) {
        fprintf(stderr, "Errore nel numero di argomenti\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    N = atoi(argv[1]);
    if (N<0){
        fprintf(stderr, "Il parametro N deve essere un intero positivo\n");
        exit(EXIT_FAILURE);
    }

    //Metto questa signal nel padre prima della creazione di P1.
    //In tal modo, non può accadere mai che P1 mandi al padre un SIGUSR1
    //prima che egli (P0) abbia settato l'handler per SIGUSR1.
    //In ogni caso, in questo modo, se P1 ricevesse un 
    //SIGUSR1, andrebbe ad eseguire father_handler perchè 
    //l'associazione segnale-handler è ereditaria.
    //Questa soluzione è possibile qui, in quanto P1 non deve gestire 
    //SIGUSR1 in maniera alternativa. 
    signal(SIGUSR1, father_handler);
    pid = fork();
    if (pid==0){ //P1
        signal(SIGALRM, alarm_handler);
        //P1: imposto il timeout e vado a avanti
        alarm(N);
        count=0;
        for(;;){
            sleep(1);
            printf("Hello world %d\n",count);
            count++;
        }
    }else if ( pid > 0 ) { //P0
        wait_child();
    }else{
        perror("Unable to fork");
        exit(EXIT_FAILURE);
    }

    
}
