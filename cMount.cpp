//
// Created by javierg on 8/06/22.
//

#include "cMount.h"

#include <iostream>
#include <string.h>

#include "estructuras.h"
#include "StructsDatos.h"
#include "metodosGenerales.h"

using namespace std;

partitiond devolverParticion(char ruta[512], char nombre[16]);

void mount(char ruta[512], char nombre[16]) {
    partitiond part = devolverParticion(ruta, nombre); //si se econtro en primaria o secundaria
    ebr eb;
    cout<<"Montando "<<endl;

    if (part.part_fit == 'B' || part.part_fit == 'F' || part.part_fit == 'W') {//si no hay primaria, buscar en la secundaria
        mntPush(part, eb, ruta); //se ingresa la particion a la lista.
    } else {
        eb = devolverLogica(ruta, nombre);
        if (eb.part_fit == 'B' || eb.part_fit == 'F' || eb.part_fit == 'W')
            mntPush(part, eb, ruta);
        else
            puts("\t[ERROR]No se encontró la particion");
    }

}

partitiond devolverParticion(char ruta[512], char nombre[16]) {
    mbr B_mbr = leerMBR(ruta);
    //buscando la posicion de la partición logica
    partitiond particiones[4];
    particiones[0] = B_mbr.mbr_partition_1;
    particiones[1] = B_mbr.mbr_partition_2;
    particiones[2] = B_mbr.mbr_partition_3;
    particiones[3] = B_mbr.mbr_partition_4;

    bool encontrada = false;
    int i;
    for (i = 0; i < 4; i++) {
        if (strncmp(particiones[i].part_name, nombre, 16) == 0) {
            if (particiones[i].part_type == 'P') {
                encontrada = true;
                return particiones[i];
            }else{
                break;
            }
        }
    }

    if (encontrada == false) {
        partitiond reto;
        reto.part_status='0';
        reto.part_type='\0';
        reto.part_fit='\0';
        reto.part_start = 0;
        reto.part_size = 0;
        strcpy(reto.part_name,"");

        return reto;
    }
}