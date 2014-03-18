#include "utils.h"

int main() {
    int id_msg;
    int active_clients = 0;
    int total_clients = 0;
    int auto_incr = 1;
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

    printf("########## SERVER #########\n");
    while(1) {
        msgrcv(id_msg, (void*)&message, msg_size, REQ_SRV, 0);
        switch(message.req) {
            case DEM_NUM_CLT:

                message.type = REP_NUM_CLT;

                if(active_clients<NB_MAX_CLT_SIM && auto_incr <= NB_MAX_CLT) {
                    active_clients++;
                    printf("Active clients increased: %d\n", active_clients);
                    printf("Demande de num client -> %d\n", auto_incr);
                    message.num_client = auto_incr;
                    auto_incr++;
                } else {
                    message.num_client = -1;
                }
                msgsnd(id_msg, (void*)&message, msg_size, 0);
            break;
            case DEM_PANIER:
                printf("Demande d'un panier par le client %d\n", message.num_client);
                message.type = message.num_client;
                //num_client is resend, it stay the same.
                msgsnd(id_msg, (void*)&message, msg_size, 0);
            break;
            case DEM_LISTE:
                printf("Demande liste d'objets par le client %d\n", message.num_client);
                sprintf(message.list_objects, "[banane, pomme, poire]");
                message.type = message.num_client;
                msgsnd(id_msg, (void*)&message, msg_size, 0);
            break;
            case DEM_INFOS:
                switch(message.num_object) {
                    case 1:
                        message.object_price = 2;
                        message.object_qty = 7;
                    break;
                    case 2:
                        message.object_price = 4;
                        message.object_qty = 70;
                    break;
                    case 3:
                        message.object_price = 4;
                        message.object_qty = 7410;
                    break;
                }
                printf("Demande d'infos par le client %d\n sur l'objet %d\n", message.num_client, message.num_object);
                message.type = message.num_client;
                msgsnd(id_msg, (void*)&message, msg_size, 0);
            break;
            case CLT_LEAVE:
                active_clients--;
                printf("Active clients decreased: %d\n", active_clients);
                message.type = message.num_client;
                msgsnd(id_msg, (void*)&message, msg_size, 0);
                if ( !active_clients && auto_incr > NB_MAX_CLT) {
                    printf("Fermeture du magasin\n");
                    exit(0);
                }
            default:
                //envoyer message au client pas compris message
                //
            break;
        }
    }
}
