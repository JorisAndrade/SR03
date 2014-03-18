#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


int main(int argc, char **argv) {

    int sd;
    struct sockaddr_in saddr;
    int sds;
    int rc_buf;
    pid_t cpid1;
    int recvMsgSize;
    char b[3];

    sd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sd != -1) {
        bzero(&saddr, sizeof(saddr));

        /* Internet address family */
        saddr.sin_family = AF_INET;

        /* Local port */
        saddr.sin_port = htons(atoi(argv[1]));

        /* Any incoming interface */
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);

        /* Bind to the local address */
        if (bind(sd, (const struct sockaddr *) &saddr, sizeof(saddr)) != -1) {
            if(listen(sd, SOMAXCONN) !=  -1) {
                while(1) {
                    sds = accept(sd, 0, 0);
                    cpid1 = fork();
                    switch(cpid1){
                        case -1 :
					        perror("serveur proc pere : erreur creation processus fils");
					        exit(EXIT_FAILURE);
				        break;
			 	        case 0 : // Processus Fils
			 	            while (1) {
			 	                if ((recvMsgSize = recv(sds, b, 3, 0) < 0)) {
			 	                    perror("erreur reception message");
			 	                }
			 	                printf("%s", b);
			 	            }
			 	        break;
			 	        default:
			     	        switch(waitpid(cpid1, &rc_buf,WUNTRACED)) {
						        case (pid_t)-1 :
							        perror("pere : erreur waitpid fils");
						        break;
						        default:
							        if (WIFEXITED(rc_buf)) {
								        printf("pere : code retour du fils : %d\n", WEXITSTATUS(rc_buf));
							        }
						        break;
					        }
			 	        break;
                    }
                }
            }
        }
        }
}
