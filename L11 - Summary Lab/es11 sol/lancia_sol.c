#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>

int pid_P1;

void sighandler(int segnale_ricevuto){

	if(segnale_ricevuto == SIGALRM){
		//Il segnale ricevuto Ã¨ corretto, invio SIGTERM a P1
		printf("Uccido P1: %d\n", pid_P1);
		kill(pid_P1, SIGTERM);
	}

}

int main (int argc, char* argv[]){
	
	int i, status, fd;
	char buffer[100], cbuf, firstword[100];
	
	/*
	* Controllo che il numero di input sia corretto
	*/
	if(argc != 4){
		printf("Usage: %s fileout timeout fase\n", argv[0]);
		exit(1);
	}
	
	/*
	* Controllo che il file passato sia scrivibile
	*/
	fd=open(argv[1], O_WRONLY);
	if (fd<0){
		printf("File %s not writable\n", argv[1]);
		exit(2);
	}
	else close(fd);
	
	/*
	* Verifico che tutti i caratteri di argv[2] siano numeri
	*/
	for(i=0; i < strlen(argv[2]); i++){
		if(!isdigit(argv[2][i])){
			printf("%s is not a number\n", argv[2]);
			exit(0);
		}
	}

	/*
	* Controllo che l'ultima stringa sia una "PSG" o  "PN"
	*/
	if( (strcmp(argv[3], "PSG") != 0) && (strcmp(argv[3], "PN") != 0 ) ){
		printf("%s is not PSG or PN\n", argv[3]);
		exit(0);
	}
	
	/*
	* Inizio esecuzione del programma
	*/

	pid_P1=fork();
	if(pid_P1==0){
		//Processo P1

		//Apertura del file in scrittura
		fd = open( argv[1] , O_WRONLY , O_TRUNC);
		if(fd < 0){
			perror("Impossibile aprire il file\n");
			exit(0);
		}
		
		//Chiudo stdout e duplico il file descriptor
		close(1);
		dup(fd);
		
		//Eseguo il programma java
		execlp("java", "java", "-jar", "barbecue.jar", (char *) 0);
		perror("Error in execlp");
		exit(3);
		
	}
	
	//Faccio gestire SIGALRM da un handler personalizzato e imposto il timer
	signal(SIGALRM, sighandler);
	alarm(atoi(argv[2]));
	
	//Attendo la terminazione del figlio
	wait(&status);
	
	//Apertura del file in lettura
	fd = open( argv[1] , O_RDONLY);
	if(fd < 0){
		printf("Impossibile aprire il file\n");
		exit(0);
	}
	
	//filtraggio file
	while(read(fd, &cbuf, sizeof(char)) != 0){
		if(cbuf == '\n'){
			buffer[i] = '\0'; i=0;
			/*
			* Terminata lettura della riga
			* Decido se stamparla o meno
			*
			* buffer[] contiene una riga del file
			*/
			
			//All'interno di firstword[] copio la prima parola della riga
			while(buffer[i] != ' '){
				firstword[i]=buffer[i]; i++;
			}
			firstword[i]='\0';
			
			//Decido se stamparla
			if((!strcmp(argv[3], "PSG")) && (!strcmp(firstword, "PSG"))){
				printf("%s\n", buffer);
			}
			if((!strcmp(argv[3], "PN")) && (!strcmp(firstword, "PN"))){
				printf("%s\n", buffer);
			}
			i=0;
			
		}else{
			buffer[i] = cbuf; 
			i++;
		}
	}

	exit(0);
}
