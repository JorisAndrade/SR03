#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>



int main() {
	int pipefd[2], rc_buf;
	pid_t cpid1, cpid2;
	
	if (pipe(pipefd) == -1) {
    	perror("Erreur creation pipe");
        exit(EXIT_FAILURE);
    }


	cpid1 = fork();
	switch(cpid1){	
		case -1 :
			perror("erreur creation premier processus fils 1");
			exit(EXIT_FAILURE);
		break;
	 	case 0 :
		//processus fils1
			//fermeture du pipe en lecture
			close(pipefd[0]);
			
			FILE *f = NULL;
			f = fopen("input.txt", "r");
			if (f == NULL){
				perror("Erreur ouverture du fichier dans fils 1");
			}else {
				char read_buffer[249];
				char message_buffer[256];
				while(fgets(read_buffer, 249, f) != NULL) {
				    //remove the last carriage return
				    read_buffer[strlen(read_buffer) - 1] = '\0';
				    
					sprintf(message_buffer, "[%03d][%s]", strlen(read_buffer), read_buffer);
					printf("lecture fichier : %s\n", message_buffer);
					write(pipefd[1], message_buffer, strlen(message_buffer));
				}
				fclose(f);
			}
			//a la fin on ferme le descripteur en ecriture
			//ce qui libere le fils 2 (attente en read)
			close(pipefd[1]);
		break;
		default:
		// processus pere
		cpid2 = fork();
		switch(cpid2){	
			case -1 :
				perror("erreur creation premier processus fils 2");
				exit(EXIT_FAILURE);
			break;
		 	case 0 :
			//processus fils2
				//fermeture du pipe en ecriture
				close(pipefd[1]);
				//while true sur le read (bloquant)
				char read_buffer[20];
				char full_line[256];
				while (read(pipefd[0], &read_buffer, 20)) {
					printf("lecture pipe : %s\n", read_buffer);
					sleep(1);
					//lire premiere ligne recueprer nombre
					//de char puis recupere le multiple de 20
					// dans le buffer et dectecter la fin
					//print la chaine quand clean
					//reutiliser la fin pour le mettre dans le buffer
					//boucler
				}
				//fermeture du pipe en lecture
				close(pipefd[0]);
			break;
			default:
			// processus pere
				//fermeture du pipe lecture et ecriture
				close(pipefd[0]);
				close(pipefd[1]);
				// attendre processus fils et se terminer
				switch(waitpid(cpid1, &rc_buf,WUNTRACED)) {
					case (pid_t)-1 :
						perror("Waitpid fils 1");
					break;
					default:
						if (WIFEXITED(rc_buf)) {
							printf("code retour du fils 1 : %d\n", WEXITSTATUS(rc_buf));
						}
					break;
				}
				switch(waitpid(cpid2, &rc_buf,WUNTRACED)) {
					case (pid_t)-1 :
						perror("Waitpid fils 2");
					break;
					default:
						if (WIFEXITED(rc_buf)) {
							printf("code retour du fils 2 : %d\n", WEXITSTATUS(rc_buf));
						}
					break;
				}
			break;
		}
		break;
	}
	 
}
