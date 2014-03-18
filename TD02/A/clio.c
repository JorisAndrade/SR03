#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "iniobj.h" 

int main(int argc, char **argv) {
    
    init();
    
    int sd;
    struct sockaddr_in saddrcli;
    struct sockaddr_in saddrserv;
    struct hostent * hid;

    sd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sd != -1) {
        /* We put 0 in struct */
        bzero(&saddrcli, sizeof(saddrcli));

        /* Internet address family */
        saddrcli.sin_family = AF_INET;

        /* We let the system choose the port */
        saddrcli.sin_port = 0;

        /* Any incoming interface */
        saddrcli.sin_addr.s_addr = htonl(INADDR_ANY);

        /* Bind to the local address */
        if (bind(sd, (const struct sockaddr *) &saddrcli, sizeof(saddrcli)) != -1) {

            bzero(&saddrserv, sizeof(saddrserv));
            saddrserv.sin_family = AF_INET;
            saddrserv.sin_port = htons(atoi(argv[2]));

            /* Check with dns server to get the ip */
            hid = gethostbyname(argv[1]);

            /* Copy n bytes from hid->h_addr to saddrserv.sin_addr.s_addr */
            bcopy(hid->h_addr, &(saddrserv.sin_addr.s_addr), sizeof(hid->h_length));

            /* Connect créé le socket coté client si pas encore créé de ligne 16 à 28*/
            if (connect(sd, (struct sockaddr *) &saddrserv, sizeof(saddrserv)) < 0 ) {

            }

            char* string = "te";
            
            int taille = strlen(string);
            
            if (send(sd, string, taille, 0) != taille) {
                perror("erreur connect");
                exit(EXIT_FAILURE);
            }
            
            
            printf("ici le client a bien send");

        }
    }
}
