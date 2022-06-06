//
// Created by javierg on 5/06/22.
//

#include "cMkdisk.h"
#include <iostream>
#include <string.h>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <time.h>

#include "estructuras.h"

using namespace std;

void execMKDISK(char rut[512], int tamano, char principal);

string getDirectorio(char direccion[512]){
    string aux = direccion;
    string delimiter = "/";
    size_t pos = 0;
    string res = "";
    while((pos = aux.find(delimiter))!=string::npos){
        res += aux.substr(0,pos)+"/";
        aux.erase(0,pos + delimiter.length());
    }
    return res;
}

void cMkdisk(int size, char name[64], char path[512]){

    //Evaluando size
    if(size>0){
        if((size%8)!=0){
            cout<<"Error: El parámetro \"$size\" solo recibe numeros positivos multiplos de 8"<<endl;
            return;
        }
    }else{
        cout<<"Error: El parámetro \"$size\" solo recibe numeros positivos"<<endl;
        return;
    }
    //*******************

    //Evaluando name
    string name_p = "";
    name_p+=name;
    string ncomando = name_p.substr(0, name_p.find("."));
    char solo_nombre[64];
    strcpy(solo_nombre,name_p.c_str());
    for (char c : ncomando) {
        if(!isalnum(c) && !isalpha(c) && c!='_'){
            cout<<"Error: El nombre debe solo puede contener numeros, letras y '_'."<<endl;
            return;
        }
    }
    int pos = name_p.find(".");
    name_p.erase(0, 1 + pos);
    transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
    if(name_p!="dsk"){
        cout<<"Error: El nombre debe contener la extensión \".dsk\" al final"<<endl;
        return;
    }
    //*******************

    //string pass = "echo $(zenity --password --title=\"Enter sudo password\") | sudo -S mkdir";
    //system(pass.c_str());

    char rut[512]="";
    strcpy(rut,path);
    strcat(rut,name);
    execMKDISK(rut,size, '1');

    char rut2[512]="";
    strcpy(rut2,path);
    strcat(rut2,ncomando.c_str());
    strcat(rut2,"_rd.dsk");
    execMKDISK(rut2,size,'0');
}

void execMKDISK(char rut[512], int tamano, char principal){

    FILE *f;
    int size  = 1024 * 1024 * tamano;

    //cout<<size;
    //echo $(zenity --password --title="Enter sudo password") | sudo -S [your command]


    string aux = getDirectorio(rut);
    string crear = "sudo mkdir -p \'"+aux+"\'";
    system(crear.c_str());
    //cout<<aux<<endl;
    string compermiso = "sudo chmod -R 777 \'"+aux+"\'";
    system(compermiso.c_str());

    if ((f = fopen(rut, "w+b")) == NULL) {
        printf("[ERROR]error al crear el disco!\n");
    } else {
        srand(time(NULL));
        mbr p;


        p.mbr_fecha_creacion=time(0);
        p.mbr_tamano=size;
        p.mbr_dsk_signature=(rand()%99);

        strcpy(p.mbr_partition_1.part_status,"desactive");
        strcpy(p.mbr_partition_2.part_status,"desactive");
        strcpy(p.mbr_partition_3.part_status,"desactive");
        strcpy(p.mbr_partition_4.part_status,"desactive");

        strcpy(p.mbr_partition_1.part_type,"");
        strcpy(p.mbr_partition_2.part_type,"");
        strcpy(p.mbr_partition_3.part_type,"");
        strcpy(p.mbr_partition_4.part_type,"");

        strcpy(p.mbr_partition_1.part_fit,"");
        strcpy(p.mbr_partition_2.part_fit,"");
        strcpy(p.mbr_partition_3.part_fit,"");
        strcpy(p.mbr_partition_4.part_fit,"");

        p.mbr_partition_1.part_start = 0;
        p.mbr_partition_2.part_start = 0;
        p.mbr_partition_3.part_start = 0;
        p.mbr_partition_4.part_start = 0;

        p.mbr_partition_1.part_size = 0;
        p.mbr_partition_2.part_size = 0;
        p.mbr_partition_3.part_size = 0;
        p.mbr_partition_4.part_size = 0;

        strcpy(p.mbr_partition_1.part_name,"");
        strcpy(p.mbr_partition_2.part_name,"");
        strcpy(p.mbr_partition_3.part_name,"");
        strcpy(p.mbr_partition_4.part_name,"");

        //escribir el mbr
        fwrite(&p, sizeof (mbr), 1, f);

        //llenando los espacios en blanco
        char vacio = '\0';
        int i = 0;

        for (i = sizeof (mbr); i < size; i++) {
            fwrite(&vacio, 1, 1, f);
        }

        fclose(f);

        if(principal=='1')
            puts("\t...................El disco fue creado..............");
    }
}
