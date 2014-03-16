#ifndef UTILS_H
#define UTILS_H

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
#define DEM_LISTE 3
#define DEM_INFOS 4
#define CLT_LEAVE 5

//Limitations
#define NB_MAX_CLT_SIM 2
#define NB_MAX_CLT 4

typedef struct {
    long type;
    int req;
    int num_client;
    char list_objects[22];
    int num_object;
    int object_price;
    int object_qty;
} msg;


#endif
