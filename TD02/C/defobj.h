#ifndef DEFOBJ_H
#define DEFOBJ_H

#include <stdio.h>
#include <sys/socket.h>

typedef struct {
    char string1 [12];
    char string2 [24];
    int ii;
    int jj;
    double dd;
    int iqt;
} obj;


typedef struct {
    int type;
    int qty;
} donnees_infos;


void print_objet(obj *mon_objet){
    printf("string1 : %s\n", mon_objet->string1);

    printf("string2 : %s\n", mon_objet->string2);

    printf("ii : %d\n", mon_objet->ii);

    printf("jj : %d\n", mon_objet->jj);

    printf("dd : %f\n", mon_objet->dd);
}

void rcv_new_msg (int sd, void * target, int size) {
    // Réception de la taille du message
    int recvMsgSize = recv(sd, target, size, 0);
    if (recvMsgSize <= 0) {
        perror("erreur reception message ou serveur deconnecté");
    }
}

void sd_new_msg (int sds, void * target, int size) {
    if (send(sds, target, size, 0) != size) {
        perror("erreur lors de l'envoi d'un message");
        exit(EXIT_FAILURE);
    }
}

#endif
