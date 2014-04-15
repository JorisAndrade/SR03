#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include "iniobj.h"

int main(int argc, char **argv) {

    init();
    obj_tab[tablen - 1].iqt = 1;
    donnees_infos infos;
    int sd;
    struct sockaddr_in saddrcli;
    struct sockaddr_in saddrserv;
    struct hostent * hid;

    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1) {
        perror("erreur creation socket client\n");
        exit(EXIT_FAILURE);
    }

    /* We put 0 in struct */
    bzero(&saddrcli, sizeof(saddrcli));

    /* Internet address family */
    saddrcli.sin_family = AF_INET;

    /* We let the system choose the port */
    saddrcli.sin_port = 0;

    /* Any incoming interface */
    saddrcli.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bind to the local address */
    if (bind(sd, (const struct sockaddr *) &saddrcli, sizeof(saddrcli)) == -1) {
        perror("client erreur binding the socket\n");
        exit(EXIT_FAILURE);
    }

    bzero(&saddrserv, sizeof(saddrserv));
    saddrserv.sin_family = AF_INET;
    saddrserv.sin_port = htons(atoi(argv[2]));

    /* Check with dns server to get the ip */
    hid = gethostbyname(argv[1]);

    /* Copy n bytes from hid->h_addr to saddrserv.sin_addr.s_addr */
    bcopy(hid->h_addr, &(saddrserv.sin_addr.s_addr), sizeof(hid->h_length));

    /* Connect créé le socket coté client si pas encore créé de ligne 16 à 28*/
    if (connect(sd, (struct sockaddr *) &saddrserv, sizeof(saddrserv)) == -1 ) {
        perror("erreur connection to the server\n");
        exit(EXIT_FAILURE);
    }

    int i;

    for (i = 0 ; i < tablen ; i++) {
        if (send(sd, &obj_tab[i], sizeof(obj_tab[i]), 0) != sizeof(obj_tab[i])) {
            perror("erreur envoie de l'objet");
            exit(EXIT_FAILURE);
        }

        do {
            // Réception de la taille et qty de l'attributs
            rcv_new_msg(sd, &infos, sizeof(infos));

            printf("Rcv : type : %d  qty : %d\n", infos.type, infos.qty);

            char * rcv_string;
            int * rcv_int;
            double * rcv_double;

            switch(infos.type) {
                case 0: //char
                    rcv_string = (char*) malloc((sizeof(char) * infos.qty) + 1);
                    printf("OKKKK %d\n", sizeof(rcv_string));
                    rcv_new_msg(sd, &rcv_string, sizeof(char) * infos.qty);
                    printf("Rcv char : %s \n", rcv_string);
                    rcv_string = malloc(sizeof(char) * infos.qty);
                    free(rcv_string);
                break;
                case 1: //int
                    rcv_int = (int*) malloc(sizeof(int) * infos.qty);
                    rcv_new_msg(sd, &rcv_int, sizeof(int) * infos.qty);
                    printf("Rcv int : %d \n", *rcv_int);
                    free(rcv_int);
                break;
                case 2: // double
                    rcv_double = (double*) malloc(sizeof(double) * infos.qty);
                    rcv_new_msg(sd, &rcv_double, sizeof(double) * infos.qty);
                    printf("Rcv double : %f \n", *rcv_double);
                    free(rcv_double);
                break;
                case -1:
                    printf("Fin de Réception de l'objet\n\n");
                break;
                default: perror("réception d'un type invalide");
            }

        } while (infos.qty != -1);
    }

    printf("Fin du client\n");

}
