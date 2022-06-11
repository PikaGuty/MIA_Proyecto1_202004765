//
// Created by javierg on 6/06/22.
//

#ifndef MIA_PROYECTO1_202004765_CFDISK_H
#define MIA_PROYECTO1_202004765_CFDISK_H

#include "estructuras.h"
#include "StructsDatos.h"

void cFdisk_crear(int size, char unit[16], char path[512], char type[16], char fit[16], char name[64]);
void cFdisk_eliminar(char path[512], char delet[16], char name[64]);
void cFdisk_add(char unit[16], char path[512], char name[64], int add);
void cFdisk_mover(char path[512], char name[64], char mov[64]);
void actualizarMBR(mbr nuevo, char ruta[512]);
void cFdisk_mover(char path[512], char name[64], char mov[64]);
prtLogica buscarLogica(char path[512], char name[64], partitiond particiones[4], int i);
ebr devLogica(char path[512], char name[64]);
void actualizarLogica(char path[512], ebr logica);

#endif //MIA_PROYECTO1_202004765_CFDISK_H
