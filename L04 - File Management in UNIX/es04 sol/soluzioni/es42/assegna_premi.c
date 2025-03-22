#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


#define MAX_STRING_LENGTH 256

void handler(int signo);

void print_usage(char *prog_name);
void figlio1(char *input, char*temp, char *output);
void figlio2(char *input, char*temp);
void stampa_riga(char *riga, int dim);
void leggi_cognome_punteggio(int fdin, char *cogn, char *punt);
int estrai_riga_punteggi(int fdin, char *str);
void print_output(char *fin);

void wait_child();

int pid[2];
int contarighe=0;
int main(int argc, char* argv[]) {
    int fd, i;
    
    // controllo argomenti:
    if ( argc != 3 ) {
        fprintf(stderr, "Numero di parametri non valido\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if (fd=open(argv[1], O_RDONLY)<0)
    {   perror("errore nell'apertura della classifica");
        exit(EXIT_FAILURE);
    }
    close(fd);
    
    signal(SIGUSR1, handler);
    // creazione figli:
    for (i=0; i<2; i++) {
        pid[i] = fork(); 
        if ( pid[i] < 0 ) {
            perror("P0: Impossibile creare figlio");
            exit(EXIT_FAILURE);
        }else if ( pid[i] == 0 ){
            if (i==0) 
                figlio1(argv[2], "FILEINV", "VINCITORI");
            else 
                figlio2(argv[1], "FILEINV");
            exit(EXIT_SUCCESS);
        }else{
            printf("Creato figlio P%d con pid %d\n", i+1, pid[i]); 
        } 
    }

    for (i=0; i< 2; i++){
        wait_child();
    }

    // verifica (opzionale: 
    printf("\n---- CONTENUTO DEL FILE VINCITORI: ----\n");
    print_output("VINCITORI");//lettura vincitori

    return 0;
}


void figlio1(char *input, char *temp, char *output){ // legge da premi e fileinv->vincitori
    char  buf[10];
    int nread, len, fd_in, fd_temp, fd_out;
    char str[MAX_STRING_LENGTH], cognome[40], punteggio[40];
    
    int i, var; 
 
    pause(); // attesa notifica da P2
    
    fd_in = open(input, O_RDONLY); // apertura premi
    if (fd_in<0)
    { perror("errore");
      exit(-1);
    }
    fd_temp=open(temp, O_RDONLY);
    if (fd_temp<0)
    { perror("errore");
      exit(-1);
    }
    fd_out=creat(output, 0777);
    
    if (fd_out<0)
    { perror("errore");
      exit(-1);
    }
    
    i=1;
    while(read(fd_in,&var,sizeof(int))>0)  /* Leggo il prossimo premio*/ 
    {   leggi_cognome_punteggio(fd_temp, cognome, punteggio); 
        sprintf(str,"il sig. %s ha vinto il premio n.%d (euro %d) con il punteggio %s\n", cognome, i++, var, punteggio);
        len=strlen(str);
        write(fd_out, &str, len);
    }
    close(fd_in);
    close(fd_temp);
    close(fd_out);
    //unlink(temp); // cancello il file di appoggio
    

}

void figlio2(char *filein, char *fileout)// filein punteggi, fileout temporaneo
{   int fd_in, fd_out, dim=-1;
    char str[80], riga[80];
    
    
    sleep(1);
    fd_in = open(filein, O_RDONLY); // apertura premi
    
    if (fd_in<0)
    { perror("errore");
      exit(-1);
    } 
    
    fd_out=creat(fileout, 0777); // creo il file temporaneo
    
    if (fd_out<0)
    { perror("errore");
      exit(-1);
    }
    
    lseek(fd_in,-1, SEEK_END);// posiziono I/O pointer alla fine
    
    while (dim!=0)
    {   dim=estrai_riga_punteggi(fd_in, str);
        if (dim>0)
        {   //printf("Figlio 2: riga letta (%d caratteri):\n", dim);
            stampa_riga(str,MAX_STRING_LENGTH);
            printf("\n");
            write(fd_out, str,dim);
        }
        else
            printf("finita lettura di punteggi!\n");
    }
    close(fd_in);
    close(fd_out);
    kill(pid[0],SIGUSR1); 
}    
    
void print_output(char *input){
    
    int fd_in,  nread;
    char buf[1];
    
    fd_in = open(input, O_RDONLY);
    if (fd_in < 0) {
        perror("P0: Impossibile creare/aprire il file");
        exit(EXIT_FAILURE);
    }

    while( nread = read(fd_in, buf, 1 )>0) { /* Leggo la prossima riga*/ 
        write(1,buf,nread);
    }
    close(fd_in);
    
}

void stampa_riga(char *R, int N) // stampa un array di caratteri terminato da '\n'
{   int i, fine=0;
    for(i=0; i<N && !fine; i++)
    {   write(1, &R[i], sizeof(char));
        if (R[i]=='\n')
            fine=1;
    }
}




void leggi_cognome_punteggio(int fdin, char *cogn, char *punt) 
// legge una riga da fdin ed estrae le stringhe cognome e punteggio
{ int i,j,k, fine=0, campo=1, nread;
  char c[1];
  i=0; j=0;
  while (!fine)
  {   read(fdin, c, sizeof(char));
      switch (campo)
      {
       case 1:  if (c[0]!=' ')
                {   cogn[i++]=c[0];
                }
                else
                    campo=2; // passo al punteggio
                break;
       case 2:  if (c[0]=='\n') // ho finito la riga ed esco
                    fine=1;
                else if (c[0]!=' ') // carattere significativo
                    punt[j++]=c[0];
                break;
      }
  }
  cogn[i]='\0';
  punt[j]='\0';
  return;
}



int estrai_riga_punteggi(int fdin, char *str)// ritorna la dim di str, 0 se non ha letto nulla
{
    int pos, x, cont=0, esci=0;
    char letto;
    
    
    pos=lseek(fdin, -1, SEEK_CUR);// scarto il '\n' dell'ultima riga
    if (pos<0) // non ci sono più righe da leggere
        return 0;
    
    contarighe++;  
    // leggo a ritroso un char alla volta finchè non trovo '\n':
    while (!esci) 
    {       read(fdin, &letto, 1);
            if (letto== '\n') // ho finito la scansione a ritroso e non è l'ultima riga
            {   esci=1;
            }
            else
            {   cont++; // ho letto un char significativo
                pos=lseek(fdin, -2, SEEK_CUR);// vado al carattere precedente
                if (pos<0) // inizio della prima riga
                {   lseek(fdin, 0, SEEK_SET); // vai a inizio riga
                    esci=1;
                }
            }
            

    }
    if (contarighe>1)
        cont++;
        
    read(fdin, str, cont); // leggo l'intera riga
    str[cont]='\n'; // aggiungo il fine riga
    cont++;
        
    pos=lseek(fdin, -(cont+1), SEEK_CUR); // vai alla fine delle riga precedente
    if (pos<0) {// non c'è una riga precedente
        lseek(fdin, 0, SEEK_SET);
    }
        
    //printf("ESTRATTA RIGA n.%d (%d caratteri): ",contarighe, cont);
    //stampa_riga(str,MAX_STRING_LENGTH);
    
    return cont;
    
}



void print_usage(char *prog_name){
    printf("Usage: %s punteggi premi\n", prog_name);
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

void handler(int signo)
{   printf("P1 è stato risvegliato da P2!\n");
}
