//
// Created by javierg on 6/06/22.
//

#ifndef MIA_PROYECTO1_202004765_STRUCTSDATOS_H
#define MIA_PROYECTO1_202004765_STRUCTSDATOS_H
#include "estructuras.h"

typedef struct{
    bool correcto;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
} datos_crearFDISK;

typedef struct{
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
} partition_crearFDISK;

typedef struct{
    bool encontrado;
    ebr B_ebr;
} prtLogica;

#endif //MIA_PROYECTO1_202004765_STRUCTSDATOS_H
