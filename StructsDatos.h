//
// Created by javierg on 6/06/22.
//

#ifndef MIA_PROYECTO1_202004765_STRUCTSDATOS_H
#define MIA_PROYECTO1_202004765_STRUCTSDATOS_H
#include "estructuras.h"


typedef char times[50];


typedef struct{
    bool correcto;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
} datos_crearFDISK;

typedef struct{
    bool encontrada;
    inodo inod;
}datosBusquedaCarpeta;

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

typedef struct mnt_nodo{
    partitiond mnt_particion;//esto si es primaria o secundaria
    ebr mnt_ebr;//esto si es extendida
    times tiempo;//tiempo en que fue montada la particion
    char mnt_ruta[512];
    char mnt_id[16];
    struct mnt_nodo *siguiente;
}mnt_nodo;

typedef struct mnt_lista{
    mnt_nodo* cabeza;
}mnt_lista;

#endif //MIA_PROYECTO1_202004765_STRUCTSDATOS_H
