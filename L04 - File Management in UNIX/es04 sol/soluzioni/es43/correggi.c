#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_STRING_LENGTH 128

/*
 * Questa SOLUZIONE usa I/O Pointer distinti per P1 e P2.
 */


/***********************/
/* HANDLER DEI SEGNALI */
/***********************/
/* Handler del segnale gestito da P0*/
void handler_P0(int signo);
/* Handler dei segnali gestiti da P1*/
void handler_P1(int signo);
/* Handler del segnale gestito da P2*/
void handler_P2(int signo) ;


/**************************/
/* DICHIARAZIONE FUNZIONI */
/**************************/
void wait_child();
void print_output(char *outputpath);

/*********************/
/* VARIABILI GLOBALI */
/*********************/
int pid1,pid2, num_correzioni = 0;
int f_in, f_out;
int current_couple=0;
char outfile[80];

int main(int argc, char ** argv)  {
  int i;
  if (argc != 3) {
        fprintf(stderr, "Numero di argomenti errato.\nUsage:\n\t%s file_in file_out\n",argv[0]);
        exit(EXIT_FAILURE);
  }
  strcpy(outfile, argv[2]);
  
  signal(SIGUSR1, handler_P0); // il padre gestirà il segnale incrementando il contatore
  signal(SIGUSR2,handler_P0); // il padre gestirà SIGUSR2 uscendo dal ciclo di attesa
  
  
  pid1 = fork();
  if (pid1 == 0) {  /* Codice di P1 */
    signal(SIGALRM, handler_P1);//P2 manderà SIGALRM a P1 quando avrà finito di leggere 
    signal(SIGUSR1, handler_P1);
    signal(SIGUSR2, handler_P1);
    printf("Sono P1 %d\n", getpid());
    if ((f_in = open(argv[1], O_RDWR)) < 0){ // apertura file in modalità mista
    printf("Errore apertura file input.\n");
    exit(-1);
    }
    
    while(1){
      printf("P1: attendo il segnale da P2...\n");
      pause();
      
    }
  
    
  }else if (pid1 > 0) {
    printf("Padre %d: creato il figlio P1 %d\n", getpid(), pid1);
  }else {
    printf("Errore prima fork\n");
    exit(-1);
  }
  
  pid2 = fork();
  if (pid2 == 0) {
    int nread;
    int c[2];
    if ((f_in = open(argv[1], O_RDONLY)) < 0){ // apertura file in modalità lettura
    printf("Errore apertura file input.\n");
    exit(-1);
    }
    printf("Sono P2 %d\n", getpid());
    sleep(2);
    while( (nread = read(f_in, &c, 2*sizeof(int))) > 0 ) 
    {   printf("P2:letto a = %d e b = %d\n", c[0],c[1]);
        if (c[0]>c[1]) // coppia non ordinata
        {   printf("P2:la coppia non è ordinata -> mando SIGUSR1 a P1\n");
            kill(pid1, SIGUSR1);    
        }else{
           printf("P2:la coppia  è ordinata -> mando SIGUSR2 a P1\n");
           kill(pid1, SIGUSR2);  
        }
        sleep(1);
    }
    kill(pid1,SIGALRM); // avverto P1 che il file è finito
    close(f_in);
    exit(0); 
  }else if (pid2 > 0) {
    printf("Padre %d: creato il figlio P2 %d\n", getpid(), pid2);
  }  else {
    printf("Errore seconda fork\n");
    exit(-1);
  }

  while(1){
      printf("padre P0 aspetta..\n");
      pause();
  }
}



void handler_P1(int signo){
  int c[2], nread, max;
  
  read(f_in, &c[0], sizeof(int));
  read(f_in, &c[1], sizeof(int));
  printf("P1, letti A= %d, B=%d\n",c[0],c[1]);
  
  if (signo==SIGUSR1) // A e B vanno scambiati
  { 
    printf("P1: la coppia A= %d, B=%d non è ordinata -> scambio!\n",c[0],c[1]);
    // torno indietro di 2:    
    lseek(f_in, -2*sizeof(int), SEEK_CUR);
    if (write(f_in, &c[1], sizeof(int)) <= 0) { // scrivo B al posto di A
          printf("Errore write.\n");
          exit(-1);
        }
    if (write(f_in, &c[0], sizeof(int)) <= 0) { // scrivo A al posto di B
          printf("Errore write.\n");
          exit(-1);
        }
    kill(getppid(), SIGUSR1); // segnale al padre la correzione
    
  }else if (signo==SIGUSR2)
  { printf("P1: la coppia A= %d, B=%d  è ordinata -> non apporto correzioni..\n",c[0],c[1]);
    
      
    }
  else if (signo==SIGALRM) // il file è finito: chiudo il file e termino
  {     printf("P1: ricevuto SIGALARM e termino!\n");
        close(f_in);
        kill(getppid(), SIGUSR2); // avverto il padre che deve terminare
        exit(0);
  }
  return;
}



void handler_P0(int signo)
{   if (signo==SIGUSR1)
    {   num_correzioni++;
        printf("Padre: effettuata %d-sima correzione.\n", num_correzioni);
        //kill(pid2, SIGUSR1); // sblocca P2
    }else if (signo==SIGUSR2)
    {   printf("Padre: fine..\n", num_correzioni);
        /* Attesa della fine dei figli */
    
        wait_child();
        wait_child();

  /* Scrittura dell'output sul file di destinazione*/
        print_output(outfile);
        exit(0);
    }
  return;
}
void handler_P2(int signo)
{   printf("P2: ho ricevuto SIGUSR1 da mio padre!\n");
}

void wait_child() {
    int pid_terminated,status;
    pid_terminated=wait(&status);
    if(WIFEXITED(status))
        printf("\nPADRE: terminazione volontaria del figlio %d con stato %d\n",
                pid_terminated, WEXITSTATUS(status));
    else if(WIFSIGNALED(status))
        printf("\nPADRE: terminazione involontaria del figlio %d a causa del segnale %d\n",
                pid_terminated,WTERMSIG(status));
}

void print_output(char *outputpath){
    printf("Totale correzioni=%d\n",num_correzioni);
    int fd_out, written;
    char buf[MAX_STRING_LENGTH];
    fd_out = creat(outputpath, 00640);
    if (fd_out < 0){
        sprintf(buf, "Impossibile aprire il file %s", outputpath);
        perror(buf);
        exit(EXIT_FAILURE);
    }

    written = write(fd_out, &num_correzioni, sizeof(int));
    if (written < 0){
        sprintf(buf, "Impossibile scrivere sul file %s", outputpath);
        perror(buf);
        exit(EXIT_FAILURE);
    }
    close(fd_out);
    return;
}
