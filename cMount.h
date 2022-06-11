//
// Created by javierg on 8/06/22.
//

#ifndef MIA_PROYECTO1_202004765_CMOUNT_H
#define MIA_PROYECTO1_202004765_CMOUNT_H

#include "estructuras.h"
#include "StructsDatos.h"
#include "metodosGenerales.h"
#include "CFdisk.h"
#include <string.h>

using namespace std;

static char metodoDeColocacionExtendida;
static mnt_lista* listaDeParticiones;

void cMount(char ruta[512], char nombre[16]);
void inicializarListaMount();
void mntVER();
void cUnmount(string id[32]);



#endif //MIA_PROYECTO1_202004765_CMOUNT_H
