//
// Created by javierg on 5/06/22.
//

#ifndef MIA_PROYECTO1_202004765_ESTRUCTURAS_H
#define MIA_PROYECTO1_202004765_ESTRUCTURAS_H

#include <iostream>
#include <string.h>
#include <time.h>

typedef struct{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
} partitiond;

typedef struct{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_dsk_signature;
    partitiond mbr_partition_1;
    partitiond mbr_partition_2;
    partitiond mbr_partition_3;
    partitiond mbr_partition_4;
} mbr;

#endif //MIA_PROYECTO1_202004765_ESTRUCTURAS_H
