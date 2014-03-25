#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include "defobj.h"


void handler_fin_fils() {
    int pid, status;
    pid = wait(&status);
    printf("handler : fin processus fils pid %d status %d\n", pid, status);
    fflush(stdout);
}



int main(int argc, char **argv) {

    int sd;
    struct sockaddr_in saddr;
    int sds;
    int rc_buf;
    pid_t cpid1;
    obj objet_buffer;

    signal(SIGCHLD, handler_fin_fils);


    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1) {
        perror("erreur creation socket client");
        exit(EXIT_FAILURE);
    }

    bzero(&saddr, sizeof(saddr));

    /* Internet address family */
    saddr.sin_family = AF_INET;

    /* Local port */
    saddr.sin_port = htons(atoi(argv[1]));

    /* Any incoming interface */
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bind to the local address */
    if (bind(sd, (const struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
        perror("serveur erreur binding the socket");
        exit(EXIT_FAILURE);
    }

    if(listen(sd, SOMAXCONN) ==  -1) {
        perror("file d'attente de connexion tcp pleine");
        exit(EXIT_FAILURE);
    }

    while(1) {
        sds = accept(sd, 0, 0);
        if (sds == -1) {
            continue;
        }
        cpid1 = fork();
        switch(cpid1){
            case -1 :
		        perror("serveur proc pere : erreur creation processus fils");
		        exit(EXIT_FAILURE);
	        break;
 	        case 0 : // Processus Fils
 	            while (1) {
                    int recvMsgSize = recv(sds, &objet_buffer, sizeof(objet_buffer), 0);

                    //simulation d'une conversation longue
                    sleep(1);

 	                if (recvMsgSize <= 0) {
 	                    perror("erreur reception message ou client deconnecté");
 	                }

                    print_objet(&objet_buffer);

                    strcpy(objet_buffer.string1, "Modified");
                    strcpy(objet_buffer.string2, "Modified by server");
                    objet_buffer.ii++;
                    objet_buffer.jj++;
                    objet_buffer.dd++;

                    int msg_size = sizeof(objet_buffer);

                    if (send(sds, &msg_size, sizeof(int), 0) != sizeof(int)) {
                        perror("erreur envoie de la taille du message");
                        exit(EXIT_FAILURE);
                    }

                    if (send(sds, &objet_buffer, sizeof(objet_buffer), 0) != sizeof(objet_buffer)) {
                        perror("erreur envoie de l'objet");
                        exit(EXIT_FAILURE);
                    }

                    if (objet_buffer.iqt == 1){
                        printf("dernier objet reçu\n");
                        exit(EXIT_SUCCESS);
                    }
 	            }
 	        break;
        }
    }
}
