#ifndef INIOBJ_H
#define INIOBJ_H

#include <stdio.h>
#include "defobj.h"

// ne doit pas depasser 10000
#define tablen 15

obj obj_tab[tablen];

void init() {
    int i;

    for (i = 0 ; i < tablen ; i++ ) {
        sprintf(obj_tab[i].string1, "ident_o%d", i);
        sprintf(obj_tab[i].string2, "description_o%d", i);

        obj_tab[i].ii = 1 + 10*(i+1);
        obj_tab[i].jj = 2 + 10*(i+1);
        obj_tab[i].dd = 0.2345 +  10*(i+1);
        obj_tab[i].iqt = 0;
    }
}

#endif
