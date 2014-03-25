#ifndef DEFOBJ_H
#define DEFOBJ_H

#include <stdio.h>


typedef struct {
    char string1 [12];
    char string2 [24];
    int ii;
    int jj;
    double dd;
    int iqt;
} obj;

void print_objet(obj *mon_objet){
    printf("string1 : %s\n", mon_objet->string1);

    printf("string2 : %s\n", mon_objet->string2);

    printf("ii : %d\n", mon_objet->ii);

    printf("jj : %d\n", mon_objet->jj);

    printf("dd : %f\n", mon_objet->dd);
}

#endif
