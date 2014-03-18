#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char **argv) {

    int sd, struct sockaddr_in saddr;
    int sds;

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
                    fork();
                    // TO DO
                }
            }
        }
}
