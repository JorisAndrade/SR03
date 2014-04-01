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
#include <unistd.h>
#include "defobj.h"


void handler_fin_fils() {
    int pid, status;
    pid = wait(&status);
    printf("handler : fin processus fils pid %d status %d\n", pid, status);
    fflush(stdout);
}



int main(int argc, char **argv) {

    int sd;
    int i;
    struct sockaddr_in saddr;
    int sds;
    int rc_buf;
    pid_t cpid1;
    donnees_infos infos;
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


                    rcv_new_msg(sds, &objet_buffer, sizeof(objet_buffer));



                    print_objet(&objet_buffer);

                    strcpy(objet_buffer.string1, "Modified");
                    strcpy(objet_buffer.string2, "Modified by server");
                    objet_buffer.ii++;
                    objet_buffer.jj++;
                    objet_buffer.dd++;

                    //Envoi string1
                    infos.type = 0;
                    infos.qty = sizeof(objet_buffer.string1)/sizeof(char);
                    sd_new_msg(sds, &infos, sizeof(infos));
                    sd_new_msg(sds, &objet_buffer.string1, sizeof(objet_buffer.string1));

                    //Envoi string2
                    infos.type = 0;
                    infos.qty = sizeof(objet_buffer.string2)/sizeof(char);
                    sd_new_msg(sds, &infos, sizeof(infos));
                    sd_new_msg(sds, &objet_buffer.string2, sizeof(objet_buffer.string2));

                    //Envoi ii
                    infos.type = 1;
                    infos.qty = sizeof(objet_buffer.ii)/sizeof(typeof(objet_buffer.ii));
                    sd_new_msg(sds, &infos, sizeof(infos));
                    sd_new_msg(sds, &objet_buffer.ii, sizeof(objet_buffer.ii));

                    //Envoi jj
                    infos.type = 1;
                    infos.qty = sizeof(objet_buffer.jj)/sizeof(typeof(objet_buffer.jj));
                    sd_new_msg(sds, &infos, sizeof(infos));
                    sd_new_msg(sds, &objet_buffer.jj, sizeof(objet_buffer.jj));

                    //Envoi dd
                    infos.type = 2;
                    infos.qty = sizeof(objet_buffer.dd)/sizeof(typeof(objet_buffer.dd));
                    sd_new_msg(sds, &infos, sizeof(infos));
                    sd_new_msg(sds, &objet_buffer.dd, sizeof(objet_buffer.dd));

                    //Envoi fin de structure
                    infos.type = -1;
                    infos.qty = -1;
                    sd_new_msg(sds, &infos, sizeof(infos));

                    if (objet_buffer.iqt == 1){
                        printf("dernier objet reçu\n");
                        exit(EXIT_SUCCESS);
                    }
 	            }
 	        break;
        }
    }
}
