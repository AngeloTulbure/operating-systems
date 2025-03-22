#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h> // strsignal() è definita in string.h  
#include <sys/wait.h>


void print_usage(char* prog_name){
    fprintf(stderr, "Usage:\n\t%s COMMAND PARAM\n", prog_name);
}

void sigusr_handler(int signo){
    //strsignal restituisce una breve descrizione del segnale dato il suo signal number
    printf("*P1 (pid=%d): ricevuto da P2 segnale %s\n", getpid(), strsignal(signo));
    if (signo==SIGUSR1){
        printf("*P1 (pid=%d): no - ho rilevato un problema\n", getpid());
    }else if (signo==SIGUSR2){
        printf("*P1 (pid=%d): ok - esecuzione avvenuta con successo\n", getpid());
    }else{
        printf("*P1 (pid=%d): signo=%d. Segnale inatteso!\n", getpid(), signo);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}


void wait_child(){
    int status, pid;
    pid = wait(&status);
    printf("*P0 (pid=%d): Terminato processo figlio PID=%d : ", getpid(), pid);
        if ((char)status==0) 
            printf("term. volontaria con stato %d\n", status>>8);
        else 
            printf("term. involontaria per segnale %d\n", (char)status);
}


int main(int argc, char* argv[])
{
    int pid1, pid2, pid3;
    /*
     * Controllo argomenti    
     */
    if (argc != 3) {
        fprintf(stderr, "Errore nel numero di argomenti\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    

    printf("*P0(pid=%d): genero i figli e poi li aspetto...\n",getpid());
    pid1 = fork();
    if (pid1==0){ //P1
        signal(SIGUSR1, sigusr_handler);
        signal(SIGUSR2, sigusr_handler);
        printf("*P1(pid=%d): attendo segnale da P2\n",getpid());
        pause();
        exit(EXIT_FAILURE);     
    }else if ( pid1 > 0 ) { //P0
        pid2 = fork();
        if (pid2==0){ //P2
            printf("*P2(pid=%d): genero il nipote P3\n",getpid());
            pid3=fork();
            if(pid3==0){ //P3 (nipote)
                printf("*P3(pid=%d): lancio il comando %s %s\n",getpid(),argv[1],argv[2]);
                execlp(argv[1],argv[1],argv[2],(char*)0);
                char buff[50];
                sprintf(buff, "*P3: errore nel lancio del comando %s\n",argv[1]);
                perror(buff);
                exit(EXIT_FAILURE); //EXIT_FAILURE=1 => P2 dovrà lanciare il segnale SIGUSR1
            }else if(pid3>0){ //P2
                int status, pid,exit_status;
                pid = wait(&status);
                printf("*P2 (pid=%d): Terminato processo figlio PID=%d : ",getpid(), pid);
                if ((char)status==0 ) {
                    exit_status = (status>>8);
                    printf("term. volontaria con stato %d\n", exit_status);
                    if (exit_status==0) 
                        //per convenzione tutti i comandi restituiscono 0 
                        //facendo una exit(0) o exit(EXIT_SUCCESS), se tutto è andato bene. 
                        kill(pid1, SIGUSR2);
                    else
                        //se COMMAND ha restituito un valore diverso da 0, 
                        //qualcosa deve essere andato storto
                        kill(pid1, SIGUSR1);
                }else {
                    printf("term. involontaria per segnale %d\n", (char)status);
                    //Se P3 è stato terminato da segnale, suppongo che qualcosa sia andato storto
                    kill(pid1, SIGUSR1);
                }
            }else{
                perror("*P2: Unable to fork for P3");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }else if (pid2 > 0) {// P0
            wait_child();
            wait_child();
        }else{
            perror("*P0: Unable to fork for P2");
            exit(EXIT_FAILURE);
        }
    }else{
        perror("*P0: Unable to fork for P1");
        exit(EXIT_FAILURE);
    }

    
}
