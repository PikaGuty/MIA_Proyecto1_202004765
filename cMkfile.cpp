//
// Created by javierg on 12/06/22.
//

#include "cMkfile.h"

#include <iostream>
#include "cMkdir.h"

using namespace std;

void cMkfile(char path[512], char id[16], bool p, int size, char cont[512]){
    //particionMontada m= devolverParticionMontada(id);
    superBloque sb = sb_retornar(id); //Obteniendo super bloque
    mnt_nodo mountNodo = retornarNodoMount(id); //Obteniendo nodo de la partición montada

    if(strcmp(mountNodo.mnt_ruta,"") == 0||sb.s_inodes_count==-1){
        return;
    }

    string auxf = path;
    size_t pos = 0;
    string res = "";
    auxf.erase(0, pos+1);
    int n = sb.s_inodes_count; //Total de inodos
    inodo ino[n];
    inodos_leer(sb.s_inode_start,n,mountNodo.mnt_ruta,ino); //Obteniendo la lista de inodos en el sistema

    inodo inodoActual=ino[0];
    datosBusquedaCarpeta busqueda, creada;
    busqueda.encontrada= false;
    char carpe[32]="";
    while ((pos = auxf.find("/")) != string::npos) {
        //cout<<"Carpeta "<<auxf.substr(0, pos)<<endl; //Carpetas que hay que verificar que existan o crearlas

        strcpy(carpe,auxf.substr(0, pos).c_str());

        busqueda=buscarCarpeta(carpe, inodoActual, p, n,mountNodo.mnt_ruta);

        auxf.erase(0, pos + 1); //eliminamos del buffer la carpeta que estabamos buscando
        if(busqueda.encontrada==true){
            inodoActual = busqueda.inod; //Cambiamos de inodo actual para seguir buscando o crear la carpeta que interesa
        }else{
            if(p==true){
                //Creación de carpetas intermedias
                creada = crearCarpeta(carpe, inodoActual, p, n,mountNodo.mnt_ruta, id);
                if (creada.encontrada==true){
                    inodoActual = creada.inod;
                }
            }else{
                cout<<"Error: No existe la carpeta "<<carpe<<endl;
                break;
            }
        }
    }

    if(busqueda.encontrada==true||(creada.encontrada==true&&p==true)) {
        strcpy(carpe, auxf.substr(0, pos).c_str());
        cout<<"CREAR ARCHIVO EN 3 2 1 ..."<<endl;
        //Hacer método para crear archivo
        //crearCarpeta(carpe, inodoActual, p, n, mountNodo.mnt_ruta, id);
    }

}