#include "utils.h"

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

    int selection = 0;

    do {
        printf("\n\n########## CLIENT n° %d #########", num_client);
        printf("\n1 - Créer un panier");
        printf("\n2 - Liste des objets disponibles");
        printf("\n3 - Informations sur un objet");
        printf("\n0 - Quit");
        printf("\nEnter a selection: ");
        scanf("%d", &selection );

        switch(selection) {
            case 1:
                message.req = DEM_PANIER;
                message.type = REQ_SRV;
                msgsnd(id_msg, (void*)&message, msg_size, 0);
                msgrcv(id_msg, (void*)&message, msg_size, num_client, 0);
                printf("\nPanier créé\n");
            break;
            case 2:
                message.req = DEM_LISTE;
                message.type = REQ_SRV;
                msgsnd(id_msg, (void*)&message, msg_size, 0);
                msgrcv(id_msg, (void*)&message, msg_size, num_client, 0);
                printf("\nRéponse : %s", message.list_objects);
            break;
            case 3:
                message.req = DEM_INFOS;
                message.type = REQ_SRV;
                int selection_objet = 0;
                do {
                    printf("\nEntrez le numéro de l'objet");
                    printf("\n1 - Banane");
                    printf("\n2 - Pomme");
                    printf("\n3 - Poire");
                    printf("\nEnter a selection: ");
                    scanf("%d", &selection_objet );
                } while (selection_objet < 1 && selection_objet > 3);
                message.num_object = selection_objet;
                msgsnd(id_msg, (void*)&message, msg_size, 0);
                msgrcv(id_msg, (void*)&message, msg_size, num_client, 0);
                printf("\nRéponse : Prix : %d, Stock: %d", message.object_price, message.object_qty);
            break;
        }
    } while (selection != 0);
}
