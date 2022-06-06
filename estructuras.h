//
// Created by javierg on 5/06/22.
//

#ifndef MIA_PROYECTO1_202004765_ESTRUCTURAS_H
#define MIA_PROYECTO1_202004765_ESTRUCTURAS_H

#include <iostream>
#include <string.h>
#include <time.h>

typedef struct{
    char part_status[16];
    char part_type[2];
    char part_fit[2];
    int part_start;
    int part_size;
    char part_name[32];
} partition;

typedef struct{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_dsk_signature;
    partition mbr_partition_1;
    partition mbr_partition_2;
    partition mbr_partition_3;
    partition mbr_partition_4;
} mbr;

#endif //MIA_PROYECTO1_202004765_ESTRUCTURAS_H
