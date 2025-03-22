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


void wait_child(int p){
    //predispongo un parametro int p col solo scopo di riportare l'indice 
    //del processo che sta eseguendo questo codice. Così posso esplicitare 
    //nella printf chi è il processo (P0, P1 o P2) che ha effettuato 
    //la wait.
    int status, pid;
    pid = wait(&status);
    printf("P%d: Terminato processo figlio PID=%d : ", p, pid);
        if ((char)status==0) 
            printf("term. volontaria con stato %d\n", status>>8);
        else 
            printf("term. involontaria per segnale %d\n", (char)status);
}


int main(int argc, char* argv[])
{
    int N, count, pid1, pid2;

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
    printf("P0(pid=%d)\n",getpid());
    pid1 = fork();
    if (pid1==0){ //P1
        printf("P1(pid=%d)\n",getpid());
        signal(SIGALRM, alarm_handler);
        //P1: imposto il timeout e vado a avanti
        alarm(N);
        count=0;
        for(;;){
            sleep(1);
            pid2=fork();
            if (pid2==0){ //P2 (nipote)
                //si noti che non occorre gestire SIGALRM in modo alternativo in P2
                //poichè i figli non ereditano i timer dai padri.
                //P2 eredita sì l'associazione SIGALRM-alarm_handler, ma non il 
                //timeout stesso impostato dalla alarm in P1
                char count_s[10];
                printf("P2(pid=%d): launching /Users/daniela/Desktop/hello %d\n",getpid(), count);
                //La exec prende in ingresso una lista di stringhe!
                //=> debbo convertire count in stringa per poterlo passare alla exec
                sprintf(count_s, "%d", count);
                execl("/Users/daniela/Desktop/hello", 
                    "/Users/daniela/Desktop/hello", count_s, (char*)0);
                perror("P2: Error in launching hello");
                exit(EXIT_FAILURE);


            }else if (pid2>0){ //codice di P1
                wait_child(1);
                count++;
            }else{
                perror("P1: Unable to fork");
                exit(EXIT_FAILURE);
            }
        }
    }else if ( pid1 > 0 ) { //P0
        wait_child(0);
    }else{
        perror("P0: Unable to fork");
        exit(EXIT_FAILURE);
    }

    
}
