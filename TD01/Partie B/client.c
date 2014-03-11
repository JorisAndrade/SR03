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
    int num_client;

    clef = ftok("./sr03p020", 0);
    if (clef == -1) {
        perror("Erreur obtention clef");
        perror("ftok()");
        exit(EXIT_FAILURE);
    }
    id_msg = msgget(clef, 0);
    if(id_msg == -1) {
        perror("Erreur creation de la file de message impossible\n");
        perror("msget()");
        exit(EXIT_FAILURE);
    }
    
    //client envoie un premier message au serveur pour demander un identifiant
    //la taille du message devra etre une constante (pour simplifier)
    msg_size = sizeof(msg) - sizeof(long);
    
    message.type = REQ_SRV;
    message.req = DEM_NUM_CLT;
    msgsnd(id_msg, (void*)&message, msg_size, 0);
    msgrcv(id_msg, (void*)&message, msg_size, REP_NUM_CLT, 0);
    num_client = message.num_client;
    
    char entry = 'z';
    while(entry != 'e') {
        switch (entry) {
            case 'p':
                message.req = DEM_PANIER;
                message.type = REQ_SRV;
                message.num_client = num_client;
                msgsnd(id_msg, (void*)&message, msg_size, 0);
                msgrcv(id_msg, (void*)&message, msg_size, num_client, 0);
            break;
        }
    }
}
