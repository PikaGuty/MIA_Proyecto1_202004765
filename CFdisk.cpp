//
// Created by javierg on 6/06/22.
//

#include "CFdisk.h"
#include <iostream>
#include <string.h>
#include <algorithm>
#include <fstream>

#include "estructuras.h"
#include "StructsDatos.h"
#include "metodosGenerales.h"

using namespace std;

void execFdisk_crearPyE(datos_crearFDISK datos, char path[512]);
void execFdisk_crearL(datos_crearFDISK datos, char path[512]);
datos_crearFDISK valFdisk_crear(int size, char unit[16], char path[512], char type[16], char fit[16], char name[64], datos_crearFDISK datos);


void cFdisk_crear(int size, char unit[16], char path[512], char type[16], char fit[16], char name[64]) {
    datos_crearFDISK datos;
    //Validar datos recibidos
    datos = valFdisk_crear(size, unit,path,type,fit,name,datos);

    if(datos.correcto==true){
        if(datos.part_type!='l'){
            execFdisk_crearPyE(datos,path);//Creando particiones Primarias y Extendidas
        }else{
            //TODO particion logica
            execFdisk_crearL(datos,path);//Creando particiones logicas
        }

    }
}

datos_crearFDISK valFdisk_crear(int size, char unit[16], char path[512], char type[16], char fit[16], char name[16],datos_crearFDISK datos) {
    //Evaluando size
    if(size<=0){
        cout<<"Error: El parámetro \"$size\" solo recibe numeros positivos"<<endl;
        datos.correcto=false;
        return datos;
    }
    //*******************

    //Evaluando unit
    string str_unit="";
    str_unit+=unit;
    if(str_unit!="") {
        if (str_unit != "B" && str_unit != "b" && str_unit != "K" && str_unit != "k" && str_unit != "M" && str_unit != "m") {
            cout << "Error: El parámetro \"@unit\" solo recibe los valores: B, K y M" << endl;
            datos.correcto=false;
            return datos;
        }
    }else{
        strcpy(unit,"K");
    }

    //*******************

    //Evaluando type
    string str_type="";
    str_type+=type;
    if(str_type!="") {
        if(str_type!="P"&&str_type!="p"&&str_type!="E"&&str_type!="e"&&str_type!="L"&&str_type!="l"){
            cout<<"Error: El parámetro \"@type\" solo recibe los valores: B, K y M"<<endl;
            datos.correcto=false;
            return datos;
        }
    }else{
        strcpy(type,"P");
    }
    str_type=type;
    if(str_type=="P"||str_type=="p"){
        datos.part_type='P';
    }else if(str_type=="E"||str_type=="e"){
        datos.part_type='E';
    }else if(str_type=="L"||str_type=="l"){
        datos.part_type='L';
    }
    //*******************

    //Evaluando fit
    string str_fit="";
    str_fit+=fit;
    if(str_fit!="") {
        if (str_fit != "BF" && str_fit != "bf" && str_fit != "Bf" && str_fit != "bF" && str_fit != "FF" && str_fit != "ff" && str_fit != "Ff" && str_fit != "fF" && str_fit != "WF" && str_fit != "wf" && str_fit != "Wf" && str_fit != "Wf") {
            cout << "Error: El parámetro \"@fit\" solo recibe los valores: BF, FF y WF" << endl;
            datos.correcto=false;
            return datos;
        }
    }else{
        strcpy(fit,"WF");
    }
    str_fit=fit;
    if(str_fit=="BF"||str_fit=="Bf"||str_fit=="bF"||str_fit=="bf"){
        datos.part_fit='B';
    }else if(str_fit=="FF"||str_fit=="Ff"||str_fit=="fF"||str_fit=="ff"){
        datos.part_fit='F';
    }else if(str_fit=="WF"||str_fit=="Wf"||str_fit=="wF"||str_fit=="wf"){
        datos.part_fit='W';
    }
    //*******************

    //Evaluando name
    string name_p = "";
    name_p+=name;

    for (char c : name_p) {
        if(!isalnum(c) && !isalpha(c) && c!='_'){
            cout<<"Error: El nombre debe solo puede contener numeros, letras y '_'."<<endl;
            datos.correcto=false;
            return datos;
        }
    }
    //*******************

    int tamano = 0;
    str_unit="";
    str_unit+=unit;
    if(str_unit=="B"||str_unit=="b"){
        tamano = size;
    }else if(str_unit=="K"||str_unit=="k"){
        tamano = 1024 * size;
    }else if(str_unit=="M"||str_unit=="m"){
        tamano = 1024 * 1024 * size;
    }
    datos.part_size=tamano;
    datos.part_start=0;
    strcpy(datos.part_name,name);

    datos.correcto=true;
    return datos;
}

void execFdisk_crearPyE(datos_crearFDISK datos, char path[512]){

    mbr retorno = leerMBR(path);


    int prtActiva[] = {0,0,0,0};

    partitiond particiones[4];
    particiones[0] = retorno.mbr_partition_1;
    particiones[1] = retorno.mbr_partition_2;
    particiones[2] = retorno.mbr_partition_3;
    particiones[3] = retorno.mbr_partition_4;

    for (int i = 0; i < 4; i++) {
        if(particiones[i].part_size!=0){
            prtActiva[i]=1;
        }
    }

    string nombre = "";
    string nombre2 = "";
    for (int i = 0; i < 4; i++) {
        nombre=particiones[i].part_name;
        nombre2=datos.part_name;
        if(nombre==nombre2){
            cout<<"Error: El nombre "<<datos.part_name<<" ya está ocupado. "<<endl;
            return;
        }
    }

    string tipo="";
    tipo=datos.part_type;

    if(tipo=="E"){
        for (int i = 0; i < 4; i++) {
            tipo="";
            tipo=particiones[i].part_type;
            if(tipo=="E"){
                cout<<"Error: Solo puede haber una partición extendida por disco. "<<endl;
                return;
            }
        }
    }

    int tamanoDisponible=0;
    int tDisponible[4]={0,0,0,0};
    int particionSeleccionada=0;

    //Buscando en las 16 combinaciones de estados de las particiones
    if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==0){ //0 0 0 0
        tamanoDisponible = retorno.mbr_tamano - (sizeof (retorno) + 1);
        if (datos.part_size > tamanoDisponible) {
            cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
            return;
        } else {
            particionSeleccionada = 0;
        }
    }else if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==1) { //0 0 0 1
        tamanoDisponible = particiones[3].part_start - (sizeof (retorno) + 1);
        if (datos.part_size > tamanoDisponible) {
            cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
            return;
        } else {
            particionSeleccionada = 0;
        }
    }else if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==0) { //0 0 1 0
        //Ahora salen 2 espacios
        tDisponible[0] = particiones[2].part_start - (sizeof (retorno) + 1);
        tDisponible[3] = retorno.mbr_tamano - (particiones[2].part_start+particiones[2].part_size+1);
        if (datos.part_size > tDisponible[0]) {
            if (datos.part_size > tDisponible[3]) {
                cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                return;
            } else {
                tamanoDisponible=tDisponible[3];
                particionSeleccionada = 3;
            }
        } else {
            tamanoDisponible=tDisponible[0];
            particionSeleccionada = 0;
        }
    }else if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==1) { //0 0 1 1
        tamanoDisponible = particiones[2].part_start - (sizeof (retorno) + 1);
        if (datos.part_size > tamanoDisponible) {
            cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
            return;
        } else {
            particionSeleccionada = 0;
        }
    }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0) { //0 1 0 0
        //Ahora salen 2 espacios
        tDisponible[0] = particiones[1].part_start - (sizeof (retorno) + 1);
        tDisponible[2] = retorno.mbr_tamano - (particiones[1].part_start+particiones[1].part_size+1);
        if (datos.part_size > tDisponible[0]) {
            if (datos.part_size > tDisponible[2]) {
                cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                return;
            } else {
                tamanoDisponible=tDisponible[2];
                particionSeleccionada = 2;
            }
        } else {
            tamanoDisponible=tDisponible[0];
            particionSeleccionada = 0;
        }
    }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==1) { //0 1 0 1
        //Ahora salen 2 espacios
        tDisponible[0] = particiones[1].part_start - (sizeof (retorno) + 1);
        tDisponible[2] = particiones[3].part_start - (particiones[1].part_start+particiones[1].part_size+1);
        if (datos.part_size > tDisponible[0]) {
            if (datos.part_size > tDisponible[2]) {
                cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                return;
            } else {
                tamanoDisponible=tDisponible[2];
                particionSeleccionada = 2;
            }
        } else {
            tamanoDisponible=tDisponible[0];
            particionSeleccionada = 0;
        }
    }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0) { //0 1 1 0
        //Ahora salen 2 espacios
        tDisponible[0] = particiones[1].part_start - (sizeof (retorno) + 1);
        tDisponible[3] = retorno.mbr_tamano - (particiones[2].part_start+particiones[2].part_size+1);
        if (datos.part_size > tDisponible[0]) {
            if (datos.part_size > tDisponible[3]) {
                cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                return;
            } else {
                tamanoDisponible=tDisponible[3];
                particionSeleccionada = 3;
            }
        } else {
            tamanoDisponible=tDisponible[0];
            particionSeleccionada = 0;
        }
    }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1) { //0 1 1 1
        tamanoDisponible = particiones[1].part_start - (sizeof (retorno) + 1);
        if (datos.part_size > tamanoDisponible) {
            cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
            return;
        } else {
            particionSeleccionada = 0;
        }
    }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==0) { //1 0 0 0
        tamanoDisponible = retorno.mbr_tamano - (particiones[0].part_start+particiones[0].part_size + 1);
        if (datos.part_size > tamanoDisponible) {
            cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
            return;
        } else {
            particionSeleccionada = 1;
        }
    }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==1) { //1 0 0 1
        tamanoDisponible = particiones[3].part_start - (particiones[0].part_start+particiones[0].part_size + 1);
        if (datos.part_size > tamanoDisponible) {
            cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
            return;
        } else {
            particionSeleccionada = 1;
        }
    }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==0) { //1 0 1 0
        //Ahora salen 2 espacios
        tDisponible[1] = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_size + 1);
        tDisponible[3] = retorno.mbr_tamano - (particiones[2].part_start+particiones[2].part_size+1);
        if (datos.part_size > tDisponible[1]) {
            if (datos.part_size > tDisponible[3]) {
                cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                return;
            } else {
                tamanoDisponible=tDisponible[3];
                particionSeleccionada = 3;
            }
        } else {
            tamanoDisponible=tDisponible[1];
            particionSeleccionada = 1;
        }
    }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==1) { //1 0 1 1
        tamanoDisponible = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_size + 1);
        if (datos.part_size > tamanoDisponible) {
            cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
            return;
        } else {
            particionSeleccionada = 1;
        }
    }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0) { //1 1 0 0
        tamanoDisponible = retorno.mbr_tamano - (particiones[1].part_start+particiones[1].part_size + 1);
        if (datos.part_size > tamanoDisponible) {
            cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
            return;
        } else {
            particionSeleccionada = 2;
        }
    }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0) { //1 1 0 1
        tamanoDisponible = particiones[3].part_start - (particiones[1].part_start+particiones[1].part_size + 1);
        if (datos.part_size > tamanoDisponible) {
            cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
            return;
        } else {
            particionSeleccionada = 2;
        }
    }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0) { //1 1 1 0
        tamanoDisponible = retorno.mbr_tamano - (particiones[2].part_start+particiones[2].part_size + 1);
        if (datos.part_size > tamanoDisponible) {
            cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
            return;
        } else {
            particionSeleccionada = 3;
        }
    }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1) { //1 1 1 1
        cout<<"Error: ya se han ocupado las 4 particiones en este disco "<<endl;
        return;
    }

    cout<<"El tamaño disponible en el disco es de "<<tamanoDisponible<<" bytes"<<endl;
    cout<<"Se creará una partición con "<<datos.part_size<<" bytes"<<endl;
    cout<<"en la partición No."<<(particionSeleccionada+1)<<" del disco"<<endl;

    particiones[particionSeleccionada].part_status='0';
    particiones[particionSeleccionada].part_type=datos.part_type;
    particiones[particionSeleccionada].part_fit=datos.part_fit;
    particiones[particionSeleccionada].part_size=datos.part_size;
    strcpy(particiones[particionSeleccionada].part_name,datos.part_name);

    switch (particionSeleccionada) {
        case 0:
            particiones[particionSeleccionada].part_start=(sizeof (retorno)+1);
            break;
        case 1:
            particiones[particionSeleccionada].part_start=(particiones[0].part_start+particiones[0].part_size+1);
            break;
        case 2:
            particiones[particionSeleccionada].part_start=(particiones[1].part_start+particiones[1].part_size+1);
            break;
        case 3:
            particiones[particionSeleccionada].part_start=(particiones[2].part_start+particiones[2].part_size+1);
            break;
    }

    cout<<endl;
    cout<<"Type: "<<datos.part_type<<endl;
    cout<<"Fit: "<<datos.part_fit<<endl;
    cout<<"Start: "<<datos.part_start<<endl;
    cout<<"Size: "<<datos.part_size<<endl;
    cout<<"Name: "<<datos.part_name<<endl;
    cout<<endl;

    retorno.mbr_partition_1=particiones[0];
    retorno.mbr_partition_2=particiones[1];
    retorno.mbr_partition_3=particiones[2];
    retorno.mbr_partition_4=particiones[3];
    actualizarMBR(retorno,path);
}

void execFdisk_crearL(datos_crearFDISK datos, char path[512]){
    cout<<"ES UNA LOGICA"<<endl;
}

void cFdisk_eliminar(char path[512], char delet[16], char name[64]){

}

void cFdisk_add(char unit[16], char path[512], char name[64], int add) {

}

void cFdisk_mover(char path[512], char name[64], char mov[64]){

}
