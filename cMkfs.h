//
// Created by javierg on 11/06/22.
//

#ifndef MIA_PROYECTO1_202004765_CMKFS_H
#define MIA_PROYECTO1_202004765_CMKFS_H

#include <string.h>
#include <iostream>
#include "StructsDatos.h"
#include "cMount.h"

void cMkfs(int add, char id[16], char unit[16], char type[16]);
void sb_escribir(char ruta[512], int inicio, superBloque sb);
void jr_escribir(int inicio, int n, char ruta[512], journalie aux[]);
void bmb_leer(int inicio, int n, char ruta[512], bmBloque *aux);
void bmb_escribir(int inicio, int n, char ruta[512], bmBloque aux[]);
void bmi_escribir(int inicio, int n, char ruta[512], bmInodo aux[]);
superBloque sb_retornar(char id[16]);
void bmi_leer(int inicio, int n, char ruta[512], bmInodo *aux);
void crear_ext3(mnt_nodo mountNodo, int n, int inicioParticion);
superBloque sb_inicializar(int n, times tiempo, int inicio);
void inodos_escribir(int inicio, int n, char ruta[512], inodo aux[]);
void inodos_leer(int inicio, int n, char ruta[512], inodo *aux);
bloqueCarpeta  blocksC_leer(int inicio, int n, char ruta[512], bloqueCarpeta aux);
void blocksC_escribir(int inicio, int n, char ruta[512], bloqueCarpeta aux);
void blocksA_leer(int inicio, int n, char ruta[512], bloqueCarpeta aux);
void blocksA_escribir(int inicio, int n, char ruta[512], bloqueCarpeta aux);

#endif //MIA_PROYECTO1_202004765_CMKFS_H
