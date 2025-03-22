#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

#define dims 10
void pulisci(char *BUF);

int main()
{   int VAR, k;
    int fd;
    char buff[dims]="";

    fd=creat("premi", 0777);
    printf("immetti una sequenza di interi (uno per riga), terminata da ^D:\n");

    while (k=read(0, buff, dims)>0)
    {   pulisci(buff); // elimino \n o altri terminatori
        VAR=atoi(buff);
        //printf("ho letto %s, convertito in %d\n", buff, VAR);
        write(fd,&VAR, sizeof(int));
    }
    close(fd);
    // verifica:
    fd=open("premi",O_RDONLY);
    if (fd<0)
    {   perror("apertura fallita -");
        exit(EXIT_FAILURE);
    }
    printf("\ncontenuto del file:\n");
    while(read(fd,&VAR,sizeof(int))>0)
        printf("%d\n", VAR);
    close(fd);
    
}

void pulisci(char *BUF)
{   int i;
    for (i=0; i<dims; i++)
        if (!isdigit(BUF[i]))
        {   BUF[i]='\0';
            return;
        }
}
