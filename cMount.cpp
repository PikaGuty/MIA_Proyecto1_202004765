//
// Created by javierg on 8/06/22.
//

#include "cMount.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


using namespace std;

bool retornarN(char nombre[16], char ruta[512]);
void inicializarListaMount();
void mntPush(mnt_lista* lista, partitiond particion, ebr logica, char ruta[512]);


mnt_nodo* mntCrearNodo(partitiond particion, ebr logica, char ruta[512]);
char letraDeDisco(mnt_lista*lista, char ruta[512]);
char numeroDeDisco(mnt_lista*lista, char letra);


const string fecha(time_t now) {
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X %d-%m-%Y ", &tstruct);

    return buf;
}

void cMount(char ruta[512], char nombre[16]) {
    partitiond part = devolverParticion(ruta, nombre); //si se econtro en primaria o secundaria
    ebr eb;
    eb.part_fit='\0';
    strcpy(eb.part_name,"");
    eb.part_status=0;
    eb.part_size=0;
    eb.part_start=0;

    if (part.part_fit == 'B' || part.part_fit == 'F' || part.part_fit == 'W') {//si no hay primaria, buscar en la secundaria
        bool existe = retornarN(nombre,ruta);
        if(existe==false) {
            mntPush(listaDeParticiones, part, eb, ruta); //se ingresa la particion a la lista.
            actualizarStatus(ruta, nombre, '1');
            cout << "\t...................Se ha montado la partición................" << endl;
        }else{
            cout<<"Error: La partición "<<part.part_name<<" ya está montada "<<endl;
        }
    } else {
        eb = devLogica(ruta,nombre);
        //eb = devolverLogica(ruta, nombre);
        if (eb.part_fit == 'B' || eb.part_fit == 'F' || eb.part_fit == 'W') {
            bool existe = retornarN(nombre,ruta);
            if(existe== false){
                mntPush(listaDeParticiones, part, eb, ruta);
                actualizarStatus(ruta, nombre, '1');
                cout << "\t...................Se ha montado la partición................" << endl;
            }else{
                cout<<"Error: La partición "<<eb.part_name<<" ya está montada"<<endl;
            }

        }else
            cout<<"Error: No se encontró la partición en el disco"<<endl;
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

    int i;
    for (i = 0; i < 4; i++) {
        if (strncmp(particiones[i].part_name, nombre, 16) == 0) {
            if (particiones[i].part_type == 'P') {
                return particiones[i];
            }else{
                break;
            }
        }
    }

    partitiond reto;
    reto.part_status='0';
    reto.part_type='\0';
    reto.part_fit='\0';
    reto.part_start = 0;
    reto.part_size = 0;
    strcpy(reto.part_name,"");

    return reto;

}

/*
ebr devolverLogica(char ruta[512], char nombre[16]) {
    mbr B_mbr = leerMBR(ruta);
    //buscando la posicion de la partición logica
    partitiond particiones[4];
    particiones[0] = B_mbr.mbr_partition_1;
    particiones[1] = B_mbr.mbr_partition_2;
    particiones[2] = B_mbr.mbr_partition_3;
    particiones[3] = B_mbr.mbr_partition_4;

    bool hayExtendida = false;
    int i;
    for (i = 0; i < 4; i++) {
        if (particiones[i].part_type == 'e') {
            hayExtendida = true;
            break;
        }
    }
    int inicio = 0;
    int tamano = 0;
    int espacioDisponible = 0;
    bool seEncontroLogica = false;

    if (hayExtendida == true) {
        //printf("\tLa partición extendida es=%i|Nombre: %s\n", i + 1, particiones[i].part_name);
        inicio = particiones[i].part_start;
        tamano = particiones[i].part_size;
        espacioDisponible = inicio + tamano;
        FILE *f;
        if ((f = fopen(ruta, "r+b")) == NULL) {
            printf("\t[ERROR]error al abrir el disco!\n");
        } else {
            ebr B_ebr;
            //printf("\tinicio = %i\n", inicio);
            fseek(f, inicio, SEEK_SET);
            fread(&B_ebr, sizeof (ebr), 1, f);
            if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                //Si se encontró el primer ERB
                int result = strncmp(B_ebr.part_name, nombre, 16);
                if (result == 0) {
                    metodoDeColocacionExtendida = particiones[i].part_fit;
                    seEncontroLogica = true;
                    return B_ebr;
                }
                int siguiente = B_ebr.part_next;

                while (true) {
                    ebr aux_ebr;
                    fseek(f, siguiente, SEEK_SET);
                    fread(&aux_ebr, sizeof (ebr), 1, f);
                    if (aux_ebr.part_fit == 'b' || aux_ebr.part_fit == 'f' || aux_ebr.part_fit == 'w') {
                        int resultado = strncmp(aux_ebr.part_name, nombre, 16);
                        if (resultado == 0) {
                            metodoDeColocacionExtendida = particiones[i].part_fit;
                            seEncontroLogica = true;
                            return aux_ebr;
                        } else {
                            siguiente = aux_ebr.part_next;
                        }
                        //es que si hay siguiente
                    } else {
                        break;
                    }
                }
            }
            //else NO hay primer EBR
            fclose(f);
        }
    }
    ebr retor;
    return retor;

}
*/

void inicializarListaMount() {
    listaDeParticiones = (mnt_lista*) malloc(sizeof (mnt_lista)); //inicializando las listas
    listaDeParticiones->cabeza = NULL;
}

void mntVER() {
    if(listaDeParticiones->cabeza!=NULL){
        mnt_nodo* puntero = listaDeParticiones->cabeza;
        while (puntero->siguiente) {
            cout<<puntero->mnt_ebr.part_name;
            cout<<puntero->mnt_particion.part_name<<endl;
            cout<<puntero->mnt_id<<endl;
            cout<<puntero->mnt_ruta<<endl;
            cout<<"  *"<<endl;
            cout<<"  *"<<endl;
            cout<<" ***"<<endl;
            cout<<"  *"<<endl;
            puntero = puntero->siguiente;
        }
        cout<<puntero->mnt_ebr.part_name;
        cout<<puntero->mnt_particion.part_name<<endl;
        cout<<puntero->mnt_id<<endl;
        cout<<puntero->mnt_ruta<<endl;
    }else{
        cout<<"No hay particiones montadas"<<endl;
    }

}

void mntPush(mnt_lista* lista, partitiond particion, ebr logica, char ruta[512]) {
    mnt_nodo* nodo = mntCrearNodo(particion, logica, ruta);
    if (lista->cabeza == NULL) {
        //Lista vacia
        lista->cabeza = nodo;
    } else {
        //Lista llena
        mnt_nodo* puntero = lista->cabeza;
        while (puntero->siguiente) {
            puntero = puntero->siguiente;
        }
        puntero->siguiente = nodo;
    }
}

mnt_nodo* mntCrearNodo(partitiond particion, ebr logica, char ruta[512]) {
    mnt_nodo* nodo = (mnt_nodo*) malloc(sizeof (mnt_nodo));
    nodo->mnt_particion = particion;
    nodo->mnt_ebr = logica;
    strcpy(nodo->mnt_ruta, ruta);

    char resultado = letraDeDisco(listaDeParticiones, ruta);
    strcpy(nodo->mnt_id, "vd00"); //aquí tengo que recorrere la lista para colocarle el id que se necesita
    nodo->mnt_id[2] = resultado;
    nodo->mnt_id[3] = numeroDeDisco(listaDeParticiones, resultado);

    time_t tiem = time(0);

    srand(time(NULL));
    strcpy(nodo->tiempo, fecha(time(0)).c_str());

    nodo->siguiente = NULL;
    return nodo;
}

char letraDeDisco(mnt_lista*lista, char ruta[512]) {
    mnt_nodo*puntero = lista->cabeza;
    char letraTemporal = 'a';
    int retorno = 0;
    if (lista->cabeza == NULL) {
        return 'a';
    }
    while (puntero) {
        retorno = strncmp(ruta, puntero->mnt_ruta, 512);
        if (retorno == 0) {
            char reto = puntero->mnt_id[2];
            return reto;
            //si lo encontro retorna 0
        } else {
            int resul = strcmp(&letraTemporal, &puntero->mnt_id[2]);
            if ((puntero->mnt_id[2]) > letraTemporal)
                letraTemporal = puntero->mnt_id[2]; //se va buscando el mas grande
        }
        puntero = puntero->siguiente;
    };

    if (retorno == 0) {
        return 'a';

    } else {
        return letraTemporal + 1;
    }
}

char numeroDeDisco(mnt_lista*lista, char letra) {
    mnt_nodo*puntero = lista->cabeza;
    char letraTemporal = '1';
    int retorno = 1;
    if (lista->cabeza == NULL) {
        return '1';
    }
    while (puntero) {
        if (letra == puntero->mnt_id[2])
            retorno = 0;
        //retorno= strcmp(&letra,&puntero->mnt_id[2]);//retorna 0 si son iguales
        if (retorno != 0) {

            if ((puntero->mnt_id[3]) > letraTemporal)
                letraTemporal = puntero->mnt_id[3]; //se va buscando el mas grande

            //si lo encontro retorna 0
        }
        puntero = puntero->siguiente;
    };

    if (retorno != 0) {
        return '1';
    } else {
        return letraTemporal + 1;
    }

}

void actualizarStatus(char path[512], char name[16], char status){
    mbr B_mbr = leerMBR(path);
    partitiond particiones[4];
    particiones[0] = B_mbr.mbr_partition_1;
    particiones[1] = B_mbr.mbr_partition_2;
    particiones[2] = B_mbr.mbr_partition_3;
    particiones[3] = B_mbr.mbr_partition_4;

    int i;
    char nombre[16];
    char nombre2[16];

    bool encontrado = false;
    for (i = 0; i < 4; i++) {
        strcpy(nombre, particiones[i].part_name);
        strcpy(nombre2, name);

        if (strncmp(nombre2, nombre, 16) == 0) {
            encontrado = true;
            break;
        }
    }

    ebr logic;
    bool encLog = false;

    if(encontrado==false){
        for (int j = 0; j < 4; j++) {
            if (particiones[j].part_type=='E') {
                prtLogica log = buscarLogica(path,name,particiones,j);
                encLog = log.encontrado;
                logic = log.B_ebr;
                break;
            }
        }
    }


    char nombreAnterior[16];
    strcpy(nombreAnterior,logic.part_name);

    string auxf = path;
    size_t pos = 0;
    string res = "";
    while ((pos = auxf.find("/")) != string::npos) {
        res += auxf.substr(0, pos) + "/";
        auxf.erase(0, pos + 1);
    }

    string nombree = "";
    pos = auxf.find(".");
    nombree += auxf.substr(0, pos);
    auxf.erase(0, pos + 1);

    char ruta2[512] = "";
    strcpy(ruta2, res.c_str());
    strcat(ruta2, nombree.c_str());
    strcat(ruta2, "_rd.dsk");


    if (encontrado == true) {
        cout << "Actualizando Status de la partición " << i + 1 << ", Nombre: " << particiones[i].part_name << endl;

        if(particiones[i].part_type=='P'){
            particiones[i].part_status = status;
        }

        if (i == 0) {
            B_mbr.mbr_partition_1 = particiones[i];
        } else if (i == 1){
            B_mbr.mbr_partition_2 = particiones[i];
        } else if (i == 2){
            B_mbr.mbr_partition_3 = particiones[i];
        }else if (i == 3) {
            B_mbr.mbr_partition_4 = particiones[i];
        }
        actualizarMBR(B_mbr, path);
    }else if (encLog == true) {
        cout << "Actualizando la Particion Lógica, Nombre: " << logic.part_name << endl;
        //cout<<"LOGIC "<<logic.part_status<<endl;
        logic.part_status=status;
        //cout<<logic.part_name<<" LOGIC "<<logic.part_status<<endl;
        FILE *f;
        if ((f = fopen(path, "r+b")) == NULL) {

        }else{
            fseek(f, logic.part_start, SEEK_SET);
            fwrite(&logic, sizeof (logic), 1, f);
            fclose(f);
        }
        if ((f = fopen(ruta2, "r+b")) == NULL) {

        }else{
            fseek(f, logic.part_start, SEEK_SET);
            fwrite(&logic, sizeof (logic), 1, f);
            fclose(f);
        }

    }else{
        cout << "Error: no se encontró la partición: "<<name<<endl;
    }
}

void cUnmount(string id[32]){
    for (int i = 0; i < 32; ++i) {
        if(id[i]!=""){
            char identificador[16];
            strcpy(identificador,id[i].c_str());
            //cout<<"RECIBO "<<identificador<<endl;
            mnt_nodo* puntero = listaDeParticiones->cabeza;
            mnt_nodo* pAnterior = NULL;
            bool entro= false;
            while (puntero->siguiente) {
                if(strncmp(puntero->mnt_id,identificador,16)==0){
                    if(pAnterior==NULL){
                        //cout<<"Es el primero"<<endl;

                        listaDeParticiones->cabeza = puntero->siguiente;
                        if(puntero->mnt_ebr.part_start!=0){
                            actualizarStatus(puntero->mnt_ruta,puntero->mnt_ebr.part_name,'0');
                        } else{
                            actualizarStatus(puntero->mnt_ruta,puntero->mnt_particion.part_name,'0');
                        }

                        entro=true;
                    }else{
                        //cout<<"Es de enmedio"<<endl;
                        pAnterior->siguiente=puntero->siguiente;
                        if(puntero->mnt_ebr.part_start!=0){
                            actualizarStatus(puntero->mnt_ruta,puntero->mnt_ebr.part_name,'0');
                        } else{
                            actualizarStatus(puntero->mnt_ruta,puntero->mnt_particion.part_name,'0');
                        }

                        entro=true;
                    }
                }
                pAnterior = puntero;
                puntero = puntero->siguiente;
            }
            if(entro==false){
                //cout<<"Es del final"<<endl;
                if(puntero->siguiente){
                    pAnterior->siguiente=NULL;
                    if(puntero->mnt_ebr.part_start!=0){
                        actualizarStatus(puntero->mnt_ruta,puntero->mnt_ebr.part_name,'0');
                    } else{
                        actualizarStatus(puntero->mnt_ruta,puntero->mnt_particion.part_name,'0');
                    }
                }else{
                    listaDeParticiones->cabeza = NULL;
                    if(puntero->mnt_ebr.part_start!=0){
                        actualizarStatus(puntero->mnt_ruta,puntero->mnt_ebr.part_name,'0');
                    } else{
                        actualizarStatus(puntero->mnt_ruta,puntero->mnt_particion.part_name,'0');
                    }
                }

            }

        }

    }
}

mnt_nodo retornarNodoMount(char ids[16]) {
    mnt_lista* lista = listaDeParticiones;
    mnt_nodo* puntero = lista->cabeza;
    int retorno;

    while (puntero) {
        retorno = strncmp(ids, puntero->mnt_id, 16);
        if (retorno == 0)//lo encontró
            return *puntero;
        puntero = puntero->siguiente;
    }
    cout<<"Error: No se encontró ese id"<<endl;
    mnt_nodo re;
    strcpy(re.mnt_ruta, "");
    return re;
}


bool retornarN(char nombre[16], char ruta[512]) {
    mnt_lista* lista = listaDeParticiones;
    mnt_nodo* puntero = lista->cabeza;
    int r1,r2;

    while (puntero) {
        cout<<ruta<<" "<< puntero->mnt_ruta<<endl;
        r1 = strcmp(ruta, puntero->mnt_ruta);
        //cout<<nombre<<" "<<puntero->mnt_particion.part_name<<" "<<puntero->mnt_ebr.part_name<<endl;
        if (puntero->mnt_particion.part_fit == 'B' || puntero->mnt_particion.part_fit == 'F' || puntero->mnt_particion.part_fit == 'W') {
            r2 = strcmp(nombre, puntero->mnt_particion.part_name);
        }else{
            r2 = strcmp(nombre, puntero->mnt_ebr.part_name);
        }
        cout<<nombre<<" "<<puntero->mnt_particion.part_name<<" "<<puntero->mnt_ebr.part_name<<endl;
        if ((r1 == 0&&r2 == 0))//lo encontró

            return true;
        puntero = puntero->siguiente;
    }
    return false;
}