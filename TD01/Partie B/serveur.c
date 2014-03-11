#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//Types
#define REQ_SRV 1
#define REP_NUM_CLT 2

//Requests
#define DEM_NUM_CLT 1
#define DEM_PANIER 2

typedef struct {
    long type;
    int req;
    int num_client;
} msg;

int main() {
    int id_msg;
    key_t clef;
    msg message;
    int msg_size;

    clef = ftok("./sr03p020", 0);
    if (clef == -1) {
        perror("Erreur obtention clef");
        perror("ftok()");
        exit(EXIT_FAILURE);
    }
    id_msg = msgget(clef, IPC_CREAT|IPC_EXCL|0666);
    if(id_msg == -1) {
        perror("Erreur creation de la file de message impossible\n");
        perror("msget()");
        exit(EXIT_FAILURE);
    }
    
    msg_size = sizeof(msg) - sizeof(long);
    while(1) {
        msgrcv(id_msg, (void*)&message, msg_size, REQ_SRV, 0);
        switch(message.req) {
            case DEM_NUM_CLT:
                message.type = REP_NUM_CLT;
                message.num_client = //unique;
                msgsnd(id_msg, (void*)&message, msg_size, 0);
            break;
            case DEM_PANIER:
                message.type = message.num_client;
                //num_client is resend, it stay the same.
                msgsnd(id_msg, (void*)&message, msg_size, 0);
            break;
            default:
                //envoyer message au client pas compris message
                //
            break;
        }
    }
}
