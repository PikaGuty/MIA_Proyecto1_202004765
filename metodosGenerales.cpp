//
// Created by javierg on 7/06/22.
//

#include "metodosGenerales.h"
#include <iostream>
#include <string.h>
#include <algorithm>
#include <fstream>

#include "estructuras.h"

using namespace std;

mbr leerMBR(char ruta[512]){
    //SACANDO EL NOMBRE DE LA RUTA PARA ENCONTRAR EL OTRO DISCO PARA RAID 1
    string aux = ruta;
    size_t pos = 0;
    string res = "";
    while((pos = aux.find("/"))!=string::npos){
        res += aux.substr(0,pos)+"/";
        aux.erase(0,pos + 1);
    }
    //cout<<"RUTA: "<<res<<endl;
    string nombre = "";
    pos = aux.find(".");
    nombre += aux.substr(0,pos);
    aux.erase(0,pos + 1);
    //cout<<"NOMBREE: "<<nombre<<endl;
    char ruta2[512]="";
    strcpy(ruta2,res.c_str());
    strcat(ruta2,nombre.c_str());
    strcat(ruta2,"_rd.dsk");

    mbr retorno;
    FILE *f;

    if ((f = fopen(ruta, "r+b")) == NULL) {
        if ((f = fopen(ruta2, "r+b")) == NULL) {
            cout<<"Error: no se ha podido al abrir el disco!\n";
        } else {
            fseek(f, 0, SEEK_SET);
            fread(&retorno, sizeof (mbr), 1, f);
            fclose(f);
        }
    } else {
        fseek(f, 0, SEEK_SET);
        fread(&retorno, sizeof (mbr), 1, f);
        fclose(f);
    }
    return retorno;
}

void actualizarMBR(mbr nuevo, char ruta[512]) {
    //SACANDO EL NOMBRE DE LA RUTA PARA ENCONTRAR EL OTRO DISCO PARA RAID 1
    string aux = ruta;
    size_t pos = 0;
    string res = "";
    while((pos = aux.find("/"))!=string::npos){
        res += aux.substr(0,pos)+"/";
        aux.erase(0,pos + 1);
    }
    //cout<<"RUTA: "<<res<<endl;
    string nombre = "";
    pos = aux.find(".");
    nombre += aux.substr(0,pos);
    aux.erase(0,pos + 1);
    //cout<<"NOMBREE: "<<nombre<<endl;
    char ruta2[512]="";
    strcpy(ruta2,res.c_str());
    strcat(ruta2,nombre.c_str());
    strcat(ruta2,"_rd.dsk");

    bool principal=true, secundario= true;
    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        principal= false;
    } else {
        fseek(f, 0, SEEK_SET);
        fwrite(&nuevo, sizeof (mbr), 1, f);
        fclose(f);
    }



    if ((f = fopen(ruta2, "r+b")) == NULL) {
        secundario= false;
    } else {
        fseek(f, 0, SEEK_SET);
        fwrite(&nuevo, sizeof (mbr), 1, f);
        fclose(f);
    }

    if(principal==true||secundario==true){
        cout<<"\t...................Se ha actualizado la partición................"<<endl;
    }else{
        cout<<"Error: no se ha podido al abrir el disco!\n";
    }
}