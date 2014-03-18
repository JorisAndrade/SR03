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
			perror("pere : erreur creation processus fils 1");
			exit(EXIT_FAILURE);
		break;
	 	case 0 : // Processus Fils 1

			// Fermeture du pipe en lecture
			close(pipefd[0]);
			
			FILE *f = NULL;
			f = fopen("input.txt", "r");
			if (f == NULL){
				perror("fils 1 : erreur ouverture du fichier");
				exit(EXIT_FAILURE);
			}

			char read_buffer[249];
			char message_buffer[256];
			while(fgets(read_buffer, 249, f) != NULL) {

			    // Si le dernier caratère est un '\n' on l'enlève.
			    if (read_buffer[strlen(read_buffer) - 1] == '\n') {
			    	read_buffer[strlen(read_buffer) - 1] = '\0';
			    }
			    
				sprintf(message_buffer, "[%03d][%s]", (int)strlen(read_buffer), read_buffer);
				printf("fils 1 : ecriture dans le pipe : %s\n", message_buffer);
				write(pipefd[1], message_buffer, strlen(message_buffer));

				// Pour forcer la lecture du fils 2 juste apres l'ecriture
				// on peut mettre un sleep ici
				//sleep(1);
			}
			fclose(f);
			
			// A la fin on ferme le descripteur en ecriture
			// ce qui libere le fils 2 (attente en read)
			close(pipefd[1]);
		break;
		default: // Processus Pere
			cpid2 = fork();
			switch(cpid2){	
				case -1 :
					perror("pere : erreur creation processus fils 2");
					exit(EXIT_FAILURE);
				break;
			 	case 0 : // Processus Fils 2
					// Fermeture du pipe en ecriture
					close(pipefd[1]);

					// Variable lecture et reconstitution lignes
					char read_buffer[20];
					char full_line_buffer[256];
					int char_full_line_read = 0;
					int char_read = 0;
					int line_length;
					int new_line = 1;
					
					// Pour forcer l'ecriture de plusieurs message du fils 1 dans le pipe
					// on peut mettre un sleep ici
					//sleep(1);

					while (char_read = read(pipefd[0], &read_buffer, 20)) {
						// Affichage de la lecture brute
						//printf("fils 2 : lecture pipe : %.*s\n", char_read, read_buffer);

						// Ajout des caracteres lus au buffer de ligne
						memcpy(&full_line_buffer[char_full_line_read], read_buffer, char_read);
						char_full_line_read += char_read;

						// Affichage du buffer de ligne
						//printf("fils 2 : etat buffer : %.*s\n", char_full_line_read, full_line_buffer);

						// Si le buffer de ligne contient le début d'une ligne
						// (cad etat initial ou une ligne complete vient d'etre lue)
						if (new_line == 1) {
							new_line = 0;
							sscanf(full_line_buffer, "[%03d]", &line_length);
							// 4 * '[' + 3 nombres
							line_length+=7;
						}

						// Si nous avons une ligne complete dans le buffer de ligne
						if (char_full_line_read >= line_length) {
							// Extraction de la ligne,
							// uniquement pour afficher les traces !
							//char ligne[line_length + 1];
							//memcpy(ligne, full_line_buffer, line_length);
							//ligne[line_length] = '\0';

							// Affichage de l'extraction d'une ligne complete
							//printf("fils 2 : ligne complete : %s\n", ligne);

							// Extraction du message contenu dans la ligne
							int message_length = line_length - 7;
							char message[message_length + 1];
							memcpy(message, &full_line_buffer[6], message_length);
							message[message_length] = '\0';

							// Affichage du message reçu
							printf("fils 2 : reçu >>>%s<<<\n", message);

							// Nouvelle ligne la ligne qui vient d'etre affichée
							// est enlevée du buffer
							new_line = 1;
							char_full_line_read -= line_length;
							// Engendre des bugs ?! remplacer par strcpy eventuellement (sans indiquer la taille)
							memcpy(full_line_buffer, &full_line_buffer[line_length], char_full_line_read);
						}

						
					}
					// Fermeture du pipe en lecture
					close(pipefd[0]);
				break;
				default: // Processus Pere
					// Fermeture du pipe lecture et ecriture
					close(pipefd[0]);
					close(pipefd[1]);
					// Attente passive des processus fils
					switch(waitpid(cpid1, &rc_buf,WUNTRACED)) {
						case (pid_t)-1 :
							perror("pere : erreur waitpid fils 1");
						break;
						default:
							if (WIFEXITED(rc_buf)) {
								printf("pere : code retour du fils 1 : %d\n", WEXITSTATUS(rc_buf));
							}
						break;
					}
					switch(waitpid(cpid2, &rc_buf,WUNTRACED)) {
						case (pid_t)-1 :
							perror("pere : erreur waitpid fils 2");
						break;
						default:
							if (WIFEXITED(rc_buf)) {
								printf("pere : code retour du fils 2 : %d\n", WEXITSTATUS(rc_buf));
							}
						break;
					}
					// Fin
				break;
			}
		break;
	}
	 
}