//
// Created by javierg on 8/06/22.
//

#ifndef MIA_PROYECTO1_202004765_CMOUNT_H
#define MIA_PROYECTO1_202004765_CMOUNT_H

#include "estructuras.h"
#include "StructsDatos.h"
#include "metodosGenerales.h"

static char metodoDeColocacionExtendida;
static mnt_lista* listaDeParticiones;

void cMount(char ruta[512], char nombre[16]);
void inicializarListaMount();
void mntVER();



#endif //MIA_PROYECTO1_202004765_CMOUNT_H
