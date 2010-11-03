/**
 * @file polash.c
 * @author Stortz Thomas
 * @version 1.0
 * @brief shell
 *
 * This files contains functions for the shell
 * */

/*Lancement du shell avec la librairie: LD_PRELOAD=./libpolalib.so ./polash 
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h> 
#include <ctype.h>      
#include <string.h>
#include <errno.h>   
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>

#define MAX_ARGS 4

char * PATH = "Polash:$:";

char *path; 

typedef struct{
	char *cmd_path;	
	int argc;	
	char *argv[MAX_ARGS];
	char cmd_suffix;
}commandStruct;

int runCommand(commandStruct * command1);

char *substr(char *src,int pos,int len)
{
	char *dest=NULL;
	if(len>0)
	{
		/*Allocation et mise a zero*/
		dest =calloc(len+1,1);
		/*Verification de la reussite de l'allocation*/
		if(NULL != dest)
		{
			strncat(dest,src+pos,len);
		}
	}
	return dest;
}

int main() {
	
	system("clear");
	int flag = 1;

	do {
		char buffer[256], *input;
		pid_t child;
		int childstatus, status;	//For Fork
	
		int x = 1;
		char *cmd;			

		char *command;			
		int cmdlen;

		commandStruct *command1 = (commandStruct*) malloc(sizeof(commandStruct));	
		command1->argc = 1;
		
		printf("\n%s", PATH);
		fflush(stdout);
		input = fgets(buffer, 256, stdin);

		cmdlen = strlen(input);		//Take out new line char
		if (input[cmdlen-1] == '\n') {	//
			input[cmdlen-1] = '\0';	//
		}				//

       		cmd = strtok(input, " ");
		command = cmd;

		if (strcmp(command, "exit") == 0){
			flag = 0;
		}	

		command1->cmd_path = command;
		command1->argv[0] = command1->cmd_path;
	
	        while (1)
	        {
			if (command1->argc > 5) {
				printf("\nTrop d'arguments!\n");
				break;
			}
	                cmd = strtok(NULL, " ");
	                if (cmd == NULL)
	                {
				break;
			}

			command1->argv[command1->argc] = cmd;	
			command1->argc = command1->argc+1;
		         	
	        }
		childstatus = fork();
		if (childstatus != 0) { 
			wait(&status);
		} else {
			runCommand(command1);
			return(0);
		}	

	}while (flag == 1);

	return 0;
	exit(0);
}

int runCommand(commandStruct * command1) {
	
	
	if(strcmp(command1->argv[0],"pola-i") == 0)
	{	
		if (command1->argv[1] == NULL)
		{
			printf("Pas assez d'arguments");
		}
		else if(command1->argv[2] == NULL)
		{
			printf("Acces refusé en lecture\n");
		}
		else
		{
			printf("Pola-request : Acces en lecture pour %s [n/y/r] ? ",command1->argv[2]);	
			char reponse = ' ';
			int ok=0;
			int c;
			while(!ok){	
				
				scanf("%c%*[^\n]", &reponse);
      				if(reponse == 'y')
				{
					printf("Pola-request : Acces en lecture authorise\n");	
					ok = 1;
				}	
				else if(reponse == 'n')
				{
					printf("Pola-request : Acces en lecture non-authorise\n");
					ok = 1;
				}
				else if(reponse == 'r')
				{
					ok = 1;
				}
				else
				{
					
					printf("on vous a demande de saisir un caractere précis \n");
      					printf("veuillez recommencer :\n");
					printf("Pola-request : Acces en lecture pour %s [n/y/r] ? ",command1->argv[2]);
					while ( ((c = getchar()) != '\n') && c != EOF);

				}
      						
    			}
			while ( ((c = getchar()) != '\n') && c != EOF);
			if(command1->argv[3] != NULL)
			{
				printf("Pola-request : Acces en ecriture pour %s [n/y/r] ? ",command1->argv[3]);	
				char resultat = ' ';
				ok=0;
				while(!ok){	
				
				scanf("%c%*[^\n]", &resultat);
      				if(resultat == 'y')
				{
					printf("Pola-request : Acces en ecriture authorise\n");	
					ok = 1;
				}	
				else if(resultat == 'n')
				{
					printf("Pola-request : Acces en ecriture non-authorise\n");
					ok = 1;
				}
				else if(resultat == 'r')
				{
					ok = 1;
				}
				else
				{
					printf("on vous a demande de saisir un caractere précis \n");
      					printf("veuillez recommencer :\n");
					printf("Pola-request : Acces en ecriture pour %s [n/y/r] ? ",command1->argv[3]);
					while ( ((c = getchar()) != '\n') && c != EOF);
				}
      						
    			}
			
		}
	}

	}
	else
	{
		if(command1->argv[1] == NULL && strcmp(command1->argv[0],"exit") == 1)
		{
			printf("pola error acces refusé en lecture\n");
			openlog("open",LOG_NDELAY,LOG_AUTHPRIV);
			syslog(LOG_ERR|LOG_AUTHPRIV,"Acces refuse en lecture");
			closelog();
		}
		else if (command1->argv[1][0] == '<' && command1->argv[1][strlen(command1->argv[1])-1] == '>')
		{	
			char *result = substr(command1->argv[1],1,strlen(command1->argv[1])-2);
			command1->argv[1] = result;
			printf("acces en ecriture permis\n");
			execvp(command1->cmd_path, command1->argv);
			
		}
		else if (command1->argv[1][strlen(command1->argv[1])-1] == '+')
		{	
			char *result = substr(command1->argv[1],0,strlen(command1->argv[1])-1);
			command1->argv[1] = result;
			printf("acces en ecriture permis aussi\n");
			execvp(command1->cmd_path, command1->argv);
			
		}
		else
		{
			printf("acces en lecture permis\n");
			execvp(command1->cmd_path, command1->argv);
			
		}
		
	}
	//printf("%s\n", command1->argv[0]);
	return 1;

}
