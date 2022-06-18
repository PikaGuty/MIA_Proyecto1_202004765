//
// Created by javierg on 12/06/22.
//

#ifndef MIA_PROYECTO1_202004765_CMKDIR_H
#define MIA_PROYECTO1_202004765_CMKDIR_H

#include "estructuras.h"
#include "cMount.h"
#include "cMkfs.h"
#include <math.h>
#include <time.h>
#include <unistd.h>

void cMkdir(char path[512], char id[16], bool p);
particionMontada devolverParticionMontada(char id[16]);
void crearRoot(char id[16]);
void verSB(superBloque sb);
void obtenerListaBMI(char id[16], int direccionesInodos[1024]);
datosBusquedaCarpeta buscarCarpeta(char carpeta[12], inodo inodoActual, bool p, int n, char ruta[512]);
datosBusquedaCarpeta crearCarpeta(char carpeta[12], inodo inodoActual, bool p, int n, char ruta[512], char id[16]);
void actualizarBMI(int posIni, int posAct, char id[16]);


#endif //MIA_PROYECTO1_202004765_CMKDIR_H
