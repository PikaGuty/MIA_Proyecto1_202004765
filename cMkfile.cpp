//
// Created by javierg on 12/06/22.
//

#include "cMkfile.h"

#include <iostream>
#include "cMkdir.h"

using namespace std;
void fechActuali(char fecha[128]);
void crearArchivo(char nombre[12], inodo inodoActual, int n, char ruta[512], char id[16], int size, char cont[512]);

void cMkfile(char path[512], char id[16], bool p, int size, char cont[512]){
    //particionMontada m= devolverParticionMontada(id);
    superBloque sb = sb_retornar(id); //Obteniendo super bloque
    mnt_nodo mountNodo = retornarNodoMount(id); //Obteniendo nodo de la partición montada

    if(strcmp(mountNodo.mnt_ruta,"") == 0||sb.s_inodes_count==-1){
        return;
    }

    if(size<0){
        cout<<"Error: El parámetro \"@size\" solo recibe numeros positivos"<<endl;
        return;
    }

    if(size==0 && strcmp(cont,"") == 0 ){
        cout<<"Error: El debe incluirse el parámetro \"@size\" o el \"@cont\""<<endl;
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
        crearArchivo(carpe, inodoActual, n, mountNodo.mnt_ruta, id, size, cont);
    }

}

void crearArchivo(char nombre[12], inodo inodoActual, int n, char ruta[512], char id[16], int size, char cont[512]){
    if(size!=0){

        datosBusquedaCarpeta res;
        res.encontrada=false;
        bool lleno = true; //Bandera para identificar si esta llenos los bloques que ya existen
        for (int i = 0; i < sizeof(inodoActual.i_block); i++) { //Buscando inodoAcrual
            superBloque sb = sb_retornar(id); //Obteniendo super bloque
            mnt_nodo mountNodo = retornarNodoMount(id); //Obteniendo nodo de la partición montada

            if(inodoActual.i_block[i]!=-1){ //buscando el primer ocupado
                bloqueCarpeta bloque;
                bloque = blocksC_leer(inodoActual.i_block[i], n, ruta, bloque);
                for (int j = 0; j < 4; ++j) { //Recorriendo los cuatro espacios del bloque carpeta
                    if (bloque.b_content[j].b_inodo==-1){ //Buscando el primer espacio libre
                        strcpy(bloque.b_content[j].b_name,nombre);

                        //ACTUALIZANDO BLOQUE CARPETA EXISTENTE
                        strcpy(bloque.b_content[j].b_name,nombre);
                        bloque.b_content[j].b_inodo=sb.s_first_ino;

                        blocksC_escribir(inodoActual.i_block[i],n,ruta,bloque);
                        //*****************************

                        //CREANDO NUEVO INODO
                        inodo ino;
                        times fech;
                        fechActuali(fech);
                        ino.i_uid = 1;
                        ino.i_gid = 1;
                        ino.i_size = size;
                        strcpy(ino.i_atime,fech);
                        strcpy(ino.i_ctime,fech);
                        strcpy(ino.i_mtime,fech);
                        ino.i_type='1';
                        for (int j = 0; j < 15; ++j) {
                            ino.i_block[j]=-1;
                        }
                        //ino.i_block[0]=sb.s_first_blo; //AQUI ME QUEDE
                        inodos_escribir1(sb.s_first_ino, n, mountNodo.mnt_ruta, ino);
                        res.inod=inodos_leer1(sb.s_first_ino, n, mountNodo.mnt_ruta, ino);
                        inodoActual=res.inod;
                        actualizarBMI(sb.s_inode_start, sb.s_first_ino, id);
                        //*****************************

                        //CREANDO PRIMER BLOQUE PARA EL NUEVO bloque
                        int subSize = size;
                        int nAct=0;
                        while (subSize!=0){
                            if(subSize>64){
                                char contenidoB[64];
                                strcpy(contenidoB,"");
                                for (int k = 0; k < 64; ++k) { //Generando numeros para llenar
                                    strncat(contenidoB, to_string(nAct).c_str(),63);
                                    nAct++;
                                    if(nAct==10){
                                        nAct=0;
                                    }
                                }
                                bloqueArchivo blocks;
                                strncpy(blocks.b_content,contenidoB,63);
                                blocksA_escribir(sb.s_first_blo, n, mountNodo.mnt_ruta, blocks);

                                for (int j = 0; j < 15; ++j) {
                                    if(inodoActual.i_block[j]==-1){
                                        inodoActual.i_block[j]=sb.s_first_blo;
                                        break;
                                    }
                                }

                                sb.s_free_blocks_counts--;
                                sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta)+1;

                                subSize=subSize-64;
                            }else{
                                char contenidoB[subSize];
                                strcpy(contenidoB,"");
                                for (int k = 0; k < subSize; ++k) {//Generando numeros para llenar el restante
                                    strncat(contenidoB, to_string(nAct).c_str(),63);
                                    nAct++;
                                    if(nAct==10){
                                        nAct=0;
                                    }
                                }
                                bloqueArchivo blocks;
                                strncpy(blocks.b_content,contenidoB,63);
                                blocksA_escribir(sb.s_first_blo, n, mountNodo.mnt_ruta, blocks);

                                for (int j = 0; j < 15; ++j) {
                                    if(inodoActual.i_block[j]==-1){
                                        inodoActual.i_block[j]=sb.s_first_blo;
                                        break;
                                    }
                                }

                                sb.s_free_blocks_counts--;
                                sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta)+1;

                                subSize=0;

                            }

                        }

                        //*****************************

                        inodos_escribir1(sb.s_first_ino, n, mountNodo.mnt_ruta, inodoActual);

                        //ACTUALIZANDO SUPERBLOQUE
                        sb.s_free_blocks_counts--;
                        sb.s_free_inodes_count--;
                        sb.s_first_ino=sb.s_first_ino+sizeof(inodo)+1;
                        sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta)+1;

                        int inicio=0;
                        if (mountNodo.mnt_particion.part_fit == 'B' || mountNodo.mnt_particion.part_fit == 'F' || mountNodo.mnt_particion.part_fit == 'W') {//es primaria
                            inicio = mountNodo.mnt_particion.part_start;
                        } else {//del ebr
                            inicio = mountNodo.mnt_ebr.part_start+sizeof (ebr);
                        }
                        sb_escribir(mountNodo.mnt_ruta,inicio,sb);
                        //*************************

                        lleno=false;
                        cout<<"\t................ Creando archivo "<<nombre<<" ................"<<endl;

                        break;
                    }
                }
            }
            if(lleno==false){ //Para salirse si encontró espacio
                break;
            }
        }
    }else{
        datosBusquedaCarpeta res;
        res.encontrada=false;
        bool lleno = true; //Bandera para identificar si esta llenos los bloques que ya existen
        for (int i = 0; i < sizeof(inodoActual.i_block); i++) { //Buscando inodoAcrual
            superBloque sb = sb_retornar(id); //Obteniendo super bloque
            mnt_nodo mountNodo = retornarNodoMount(id); //Obteniendo nodo de la partición montada

            if(inodoActual.i_block[i]!=-1){ //buscando el primer ocupado
                bloqueCarpeta bloque;
                bloque = blocksC_leer(inodoActual.i_block[i], n, ruta, bloque);
                for (int j = 0; j < 4; ++j) { //Recorriendo los cuatro espacios del bloque carpeta
                    if (bloque.b_content[j].b_inodo==-1){ //Buscando el primer espacio libre
                        strcpy(bloque.b_content[j].b_name,nombre);

                        //ACTUALIZANDO BLOQUE CARPETA EXISTENTE
                        strcpy(bloque.b_content[j].b_name,nombre);
                        bloque.b_content[j].b_inodo=sb.s_first_ino;

                        blocksC_escribir(inodoActual.i_block[i],n,ruta,bloque);
                        //*****************************


                        string archivo="";
                        fstream f;

                        f.open(cont,ios::in);
                        if (f.is_open()){
                            string tp;
                            while(getline(f, tp)){
                                archivo+=tp;
                                archivo+=" ";
                            }
                            f.close();
                        }else{
                            cout<<"Error: no se ha podido al abrir el archivo!\n";
                        }

                        //CREANDO NUEVO INODO
                        inodo ino;
                        times fech;
                        fechActuali(fech);
                        ino.i_uid = 1;
                        ino.i_gid = 1;
                        ino.i_size = archivo.length();
                        cout<<"Longitud "<<archivo.length();
                        strcpy(ino.i_atime,fech);
                        strcpy(ino.i_ctime,fech);
                        strcpy(ino.i_mtime,fech);
                        ino.i_type='1';
                        for (int j = 0; j < 15; ++j) {
                            ino.i_block[j]=-1;
                        }
                        //ino.i_block[0]=sb.s_first_blo; //AQUI ME QUEDE
                        inodos_escribir1(sb.s_first_ino, n, mountNodo.mnt_ruta, ino);
                        res.inod=inodos_leer1(sb.s_first_ino, n, mountNodo.mnt_ruta, ino);
                        inodoActual=res.inod;
                        actualizarBMI(sb.s_inode_start, sb.s_first_ino, id);
                        //*****************************

                        //CREANDO PRIMER BLOQUE PARA EL NUEVO bloque
                        int subSize = archivo.length();

                        int nAct=0;
                        while (subSize!=0){
                            if(subSize>64){
                                bloqueArchivo blocks;
                                strncpy(blocks.b_content,archivo.c_str(),65);
                                blocksA_escribir(sb.s_first_blo, n, mountNodo.mnt_ruta, blocks);

                                for (int j = 0; j < 15; ++j) {
                                    if(inodoActual.i_block[j]==-1){
                                        inodoActual.i_block[j]=sb.s_first_blo;
                                        break;
                                    }
                                }

                                sb.s_free_blocks_counts--;
                                sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta)+1;

                                subSize=subSize-65;
                            }else{
                                bloqueArchivo blocks;
                                strncpy(blocks.b_content,archivo.c_str(),65);
                                blocksA_escribir(sb.s_first_blo, n, mountNodo.mnt_ruta, blocks);

                                for (int j = 0; j < 15; ++j) {
                                    if(inodoActual.i_block[j]==-1){
                                        inodoActual.i_block[j]=sb.s_first_blo;
                                        break;
                                    }
                                }

                                sb.s_free_blocks_counts--;
                                sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta)+1;

                                subSize=0;

                            }
                            bloqueArchivo blocks;
                            archivo.erase(0, 65);
                        }

                        //*****************************

                        inodos_escribir1(sb.s_first_ino, n, mountNodo.mnt_ruta, inodoActual);

                        //ACTUALIZANDO SUPERBLOQUE
                        sb.s_free_blocks_counts--;
                        sb.s_free_inodes_count--;
                        sb.s_first_ino=sb.s_first_ino+sizeof(inodo)+1;
                        sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta)+1;

                        int inicio=0;
                        if (mountNodo.mnt_particion.part_fit == 'B' || mountNodo.mnt_particion.part_fit == 'F' || mountNodo.mnt_particion.part_fit == 'W') {//es primaria
                            inicio = mountNodo.mnt_particion.part_start;
                        } else {//del ebr
                            inicio = mountNodo.mnt_ebr.part_start+sizeof (ebr);
                        }
                        sb_escribir(mountNodo.mnt_ruta,inicio,sb);
                        //*************************

                        lleno=false;
                        cout<<"\t................ Creando archivo "<<nombre<<" ................"<<endl;

                        break;
                    }
                }
            }
            if(lleno==false){ //Para salirse si encontró espacio
                break;
            }
        }
    }
}

void fechActuali(times fecha) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    strcpy(fecha,buf);
}