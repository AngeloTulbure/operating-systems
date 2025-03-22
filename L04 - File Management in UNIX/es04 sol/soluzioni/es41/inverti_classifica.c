#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


#define MAX_STRING_LENGTH 256

void father_handler(int signo);

void print_usage(char *prog_name);
void codice_figlio(char *input, char *output);
void print_output(char *fin);

void wait_child();

int main(int argc, char* argv[]) {
    int pid, fd;
    
    // controllo argomenti:
    if ( argc != 2 ) {
        fprintf(stderr, "Numero di parametri non valido\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if (fd=open(argv[1], O_RDONLY)<0)
    {   perror("errore nell'apertura della classifica");
        exit(EXIT_FAILURE);
    }
    close(fd);
    
    // creazione figlio:
    pid = fork(); 
    if ( pid < 0 ) {
            perror("P0: Impossibile creare P1");
            exit(EXIT_FAILURE);
    }else if ( pid == 0 ){
            codice_figlio(argv[1], "SPEAKER");
            exit(EXIT_SUCCESS);
    }else{
            printf("Creato figlio  con pid %d\n", pid); 
    } 
    wait_child();
    

    print_output("SPEAKER");

    return 0;
}


void codice_figlio(char *input, char *output){
    char  buf[10];
    int nread, pos, fd_in, fd_out;
    int fine=0;
    
    fd_in = open(input, O_RDONLY);
    if (fd_in<0)
    { perror("errore");
      exit(-1);
    }
    fd_out = open(output, O_WRONLY|O_CREAT|O_TRUNC, 0777);
    if (fd_out<0)
    { perror("errore");
      exit(-1);
    }
    
    pos=lseek(fd_in, -10, SEEK_END);
    while( (nread = read(fd_in, buf, 10 )>0) && !fine) { /* Leggo la prossima riga*/ 
        write(fd_out,buf,10);
        pos=lseek(fd_in, -20, SEEK_CUR); // indietreggio di 2 righe
        if (pos<0) // fine
            fine=1;
    }
    close(fd_in);
    close(fd_out);
    
}


void print_output(char *input){
    
    int fd_in,  nread;
    char buf[10];
    
    fd_in = open(input, O_RDONLY);
    if (fd_in < 0) {
        perror("P0: Impossibile creare/aprire il file");
        exit(EXIT_FAILURE);
    }

    while( nread = read(fd_in, buf, 10 )>0) { /* Leggo la prossima riga*/ 
        write(1,buf,10);
    }
    close(fd_in);
    
}


void print_usage(char *prog_name){
    printf("Usage: %s class\n", prog_name);
}

void wait_child() {
    int pid_terminated,status;
    pid_terminated=wait(&status);
    if(WIFEXITED(status))
        printf("PADRE: terminazione volontaria del figlio %d con stato %d\n",
                pid_terminated, WEXITSTATUS(status));
    else if(WIFSIGNALED(status))
        printf("PADRE: terminazione involontaria del figlio %d a causa del segnale %d\n",
                pid_terminated,WTERMSIG(status));
}

