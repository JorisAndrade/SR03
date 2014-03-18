#ifndef INIOBJ_H
#define INIOBJ_H

#include <stdlib.h>
#include <string.h>
#include "defobj.h"

#define tablen 4

obj obj_tab[tablen];

void init() {
    strcpy(obj_tab[0].string1, "ident_01");
    strcpy(obj_tab[0].string2, "description_01");
    obj_tab[0].ii = 11;
    obj_tab[0].jj = 12;
    obj_tab[0].dd = 10.2345;

    strcpy(obj_tab[1].string1, "ident_02");
    strcpy(obj_tab[1].string2, "description_02");
    obj_tab[1].ii = 21;
    obj_tab[1].jj = 22;
    obj_tab[1].dd = 20.2345;

    strcpy(obj_tab[2].string1, "ident_03");
    strcpy(obj_tab[2].string2, "description_03");
    obj_tab[2].ii = 31;
    obj_tab[2].jj = 32;
    obj_tab[2].dd = 30.2345;

    strcpy(obj_tab[3].string1, "ident_04");
    strcpy(obj_tab[3].string2, "description_04");
    obj_tab[3].ii = 41;
    obj_tab[3].jj = 42;
    obj_tab[3].dd = 40.2345;
}

#endif
