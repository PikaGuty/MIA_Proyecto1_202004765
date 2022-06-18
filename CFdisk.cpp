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
#include "cMount.h"
#include "metodosGenerales.h"

using namespace std;

void execFdisk_crearPyE(datos_crearFDISK datos, char path[512]);
void execFdisk_crearL(datos_crearFDISK datos, char path[512]);
void Fdisk_crearL(datos_crearFDISK datos, char path[512]);
void fdVER(char path[512]);
void actualizarLogica(char path[512], char name[16], ebr logica);

datos_crearFDISK valFdisk_crear(int size, char unit[16], char path[512], char type[16], char fit[16], char name[64], datos_crearFDISK datos);


void cFdisk_crear(int size, char unit[16], char path[512], char type[16], char fit[16], char name[64]) {
    datos_crearFDISK datos;
    //Validar datos recibidos
    datos = valFdisk_crear(size, unit,path,type,fit,name,datos);

    if(datos.correcto==true){
        if(datos.part_type!='L'){
            execFdisk_crearPyE(datos,path);//Creando particiones Primarias y Extendidas
        }else{
            execFdisk_crearL(datos,path);//Creando particiones logicas
        }

    }
    //fdVER(path);
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
    mbr retorno = leerMBR(path);

    partitiond particiones[4];
    particiones[0] = retorno.mbr_partition_1;
    particiones[1] = retorno.mbr_partition_2;
    particiones[2] = retorno.mbr_partition_3;
    particiones[3] = retorno.mbr_partition_4;

    string tipo="";
    bool hayExt = false;
    int i;
    for (i = 0; i < 4; i++) {
        tipo="";
        tipo=particiones[i].part_type;
        if(tipo=="E"){
            hayExt = true;
            break;
        }
    }

    int inicio = 0;
    int tamano = 0;
    int tamanoDiponible = 0;

    if(hayExt==true) {
        cout<<"La partición extendida es la "<<i<<"del disco "<<endl;
        inicio = particiones[i].part_start;
        tamano = particiones[i].part_size;
        tamanoDiponible = inicio + tamano; //No es el espacio disponible en si, si no el ultimo byte de la partición extendida

        string auxf = path;
        size_t pos = 0;
        string res = "";
        while((pos = auxf.find("/"))!=string::npos){
            res += auxf.substr(0,pos)+"/";
            auxf.erase(0,pos + 1);
        }

        string nombre = "";
        pos = auxf.find(".");
        nombre += auxf.substr(0,pos);
        auxf.erase(0,pos + 1);

        char ruta2[512]="";
        strcpy(ruta2,res.c_str());
        strcat(ruta2,nombre.c_str());
        strcat(ruta2,"_rd.dsk");

        FILE *f;
        if ((f = fopen(path, "r+b")) == NULL) {
            if ((f = fopen(ruta2, "r+b")) == NULL) {
                cout<<"Error: no se ha podido al abrir el disco!\n";
            }else{
                ebr B_ebr;
                cout<<"Inicio: "<<inicio<<endl;
                cout<<"Tamaño extendida "<<tamano<<endl;

                fseek(f, inicio, SEEK_SET);
                fread(&B_ebr, sizeof (ebr), 1, f);
                if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                    ebr anterior;
                    anterior = B_ebr;
                    int siguiente = B_ebr.part_next;
                    ebr aux;

                    while (true){
                        fseek(f, siguiente, SEEK_SET);
                        fread(&aux, sizeof (ebr), 1, f);

                        if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente
                            siguiente = aux.part_next;
                            anterior = aux;
                        } else { //Es el ultimo
                            //Agregando la direccion al bloque anterior
                            aux.part_start = anterior.part_start + sizeof (ebr) + anterior.part_size + 1;
                            fseek(f, aux.part_start, SEEK_SET);
                            aux.part_status = '0';
                            aux.part_fit = datos.part_fit;

                            aux.part_size = datos.part_size;
                            aux.part_next = -1; //NO hay siguiente

                            strcpy(aux.part_name, datos.part_name);
                            anterior.part_next = aux.part_start;

                            if (aux.part_start + aux.part_size > tamanoDiponible) {
                                cout<<"Error: el tamaño supera el espacio disponible en la partición extendida!\n";
                                fclose(f);
                                return;
                            } else {
                                fwrite(&aux, sizeof (ebr), 1, f);
                                fseek(f, anterior.part_start, SEEK_SET);
                                fwrite(&anterior, sizeof (ebr), 1, f);
                                cout<<"Se creo la partición logica"<<endl;
                            }
                            break;
                        }
                    }
                }else {
                    fseek(f, inicio, SEEK_SET);//Creando el EBR
                    B_ebr.part_status = '0';
                    B_ebr.part_fit = datos.part_fit;
                    B_ebr.part_start = inicio;
                    B_ebr.part_size = datos.part_size;
                    B_ebr.part_next = -1;
                    strcpy(B_ebr.part_name, datos.part_name);
                    fwrite(&B_ebr, sizeof (B_ebr), 1, f);
                    cout<<"Se creó el primer EBR"<<endl;
                }
                fclose(f);
            }
        }else{
            ebr B_ebr;
            cout<<"Inicio: "<<inicio<<endl;
            cout<<"Tamaño extendida "<<tamano<<endl;

            fseek(f, inicio, SEEK_SET);
            fread(&B_ebr, sizeof (ebr), 1, f);
            if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                ebr anterior;
                anterior = B_ebr;
                int siguiente = B_ebr.part_next;
                ebr aux;

                while (true){
                    fseek(f, siguiente, SEEK_SET);
                    fread(&aux, sizeof (ebr), 1, f);

                    if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente
                        siguiente = aux.part_next;
                        anterior = aux;
                    } else { //Es el ultimo
                        //Agregando la direccion al bloque anterior
                        aux.part_start = anterior.part_start + sizeof (ebr) + anterior.part_size + 1;
                        fseek(f, aux.part_start, SEEK_SET);
                        aux.part_status = '0';
                        aux.part_fit = datos.part_fit;

                        aux.part_size = datos.part_size;
                        aux.part_next = -1; //NO hay siguiente

                        strcpy(aux.part_name, datos.part_name);
                        anterior.part_next = aux.part_start;

                        if (aux.part_start + aux.part_size > tamanoDiponible) {
                            cout<<"Error: el tamaño supera el espacio disponible en la partición extendida!\n";
                            fclose(f);
                            return;

                        } else {
                            fwrite(&aux, sizeof (ebr), 1, f);
                            fseek(f, anterior.part_start, SEEK_SET);
                            fwrite(&anterior, sizeof (ebr), 1, f);
                            cout<<"Se creo la partición logica"<<endl;
                        }
                        break;
                    }
                }
            }else {
                fseek(f, inicio, SEEK_SET);//Creando el EBR
                B_ebr.part_status = '0';
                B_ebr.part_fit = datos.part_fit;
                B_ebr.part_start = inicio;
                B_ebr.part_size = datos.part_size;
                B_ebr.part_next = -1;
                strcpy(B_ebr.part_name, datos.part_name);
                fwrite(&B_ebr, sizeof (B_ebr), 1, f);
                cout<<"Se creó el primer EBR"<<endl;
            }
            fclose(f);
        }
    }else{
        cout<<"Error: No se encontró ninguna partición extendida en el disco"<<endl;
        return;
    }

}

ebr devLogica(char path[512], char name[64]){
    mbr B_mbr = leerMBR(path);

    partitiond particiones[4];
    particiones[0] = B_mbr.mbr_partition_1;
    particiones[1] = B_mbr.mbr_partition_2;
    particiones[2] = B_mbr.mbr_partition_3;
    particiones[3] = B_mbr.mbr_partition_4;

    int i;
    char nombre[16];
    char nombre2[16];



    ebr logic;
    bool encLog = false;

    for (int j = 0; j < 4; j++) {
        if (particiones[j].part_type=='E') {
            metodoDeColocacionExtendida = particiones[j].part_fit;
            prtLogica log = buscarLogica(path,name,particiones,j);
            encLog = log.encontrado;
            logic = log.B_ebr;
            break;
        }
    }

    return logic;
}

void cFdisk_eliminar(char path[512], char delet[16], char name[64]) {
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
        if(particiones[i].part_status=='0'){
            cout << "Eliminando la Particion " << i + 1 << ", Nombre: " << particiones[i].part_name << endl;

            if (strncmp("fast", delet, sizeof("fast")) == 0) {
                cout << "Tipo = fast, size " << particiones[i].part_size << endl;

                if(particiones[i].part_type=='E'){

                    FILE *f;
                    if ((f = fopen(path, "r+b")) == NULL) {
                        if ((f = fopen(ruta2, "r+b")) == NULL) {
                            cout << "Error: no se ha podido al abrir el disco!\n";
                        } else {
                            fseek(f, particiones[i].part_start, SEEK_SET);
                            //llenando los espacios en blanco
                            char vacio = '\0';
                            int i = 0;
                            for (i = 0; i < particiones[i].part_size; i++) {
                                fwrite(&vacio, 1, 1, f);
                            }
                            fclose(f);
                        }
                    } else {
                        fseek(f, particiones[i].part_start, SEEK_SET);
                        //llenando los espacios en blanco
                        char vacio = '\0';
                        int i = 0;
                        for (i = 0; i < particiones[i].part_size; i++) {
                            fwrite(&vacio, 1, 1, f);
                        }
                        fclose(f);
                    }
                    cout << "Eliminando particiones logicas de la extendida : " << particiones[i].part_name << endl;
                }

                particiones[i].part_size = 0;
                particiones[i].part_start = 0;
                particiones[i].part_fit = ' ';
                strcpy(particiones[i].part_name, "");
                particiones[i].part_status = '0';
                particiones[i].part_type = ' ';

            } else {
                cout << "Tipo =full, size " << particiones[i].part_size << endl;

                FILE *f;
                if ((f = fopen(path, "r+b")) == NULL) {
                    if ((f = fopen(ruta2, "r+b")) == NULL) {
                        cout << "Error: no se ha podido al abrir el disco!\n";
                    } else {
                        fseek(f, particiones[i].part_start, SEEK_SET);
                        //llenando los espacios en blanco
                        char vacio = '\0';
                        int i = 0;
                        for (i = 0; i < particiones[i].part_size; i++) {
                            fwrite(&vacio, 1, 1, f);
                        }
                        fclose(f);
                    }
                } else {
                    fseek(f, particiones[i].part_start, SEEK_SET);
                    //llenando los espacios en blanco
                    char vacio = '\0';
                    int i = 0;
                    for (i = 0; i < particiones[i].part_size; i++) {
                        fwrite(&vacio, 1, 1, f);
                    }
                    fclose(f);
                }

                particiones[i].part_size = 0;
                particiones[i].part_start = 0;
                particiones[i].part_fit = ' ';
                strcpy(particiones[i].part_name, "");
                particiones[i].part_status = '0';
                particiones[i].part_type = ' ';

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
        }else{
            cout<<"Error: debe desmontar la partición para poder eliminarla"<<endl;
        }

    }else if (encLog == true) {
        cout << "Eliminando la Particion Lógica, Nombre: " << logic.part_name << endl;

        if(logic.part_status=='0') {
            if (strncmp("fast", delet, sizeof("fast")) == 0) {
                cout << "Tipo = fast, size " << logic.part_size << endl;

                logic.part_fit = '\0';
                logic.part_start = 0;
                strcpy(logic.part_name, "");
                logic.part_size = 0;
                logic.part_status = '0';

            } else {
                cout << "Tipo =full, size " << logic.part_size << endl;

                FILE *f;
                if ((f = fopen(path, "r+b")) == NULL) {
                    if ((f = fopen(ruta2, "r+b")) == NULL) {
                        cout << "Error: no se ha podido al abrir el disco!\n";
                    } else {
                        fseek(f, logic.part_start, SEEK_SET);
                        //llenando los espacios en blanco
                        char vacio = '\0';
                        int i = 0;
                        for (i = 0; i < logic.part_size; i++) {
                            fwrite(&vacio, 1, 1, f);
                        }
                        fclose(f);
                    }
                } else {
                    fseek(f, logic.part_start, SEEK_SET);
                    //llenando los espacios en blanco
                    char vacio = '\0';
                    int i = 0;
                    for (i = 0; i < logic.part_size; i++) {
                        fwrite(&vacio, 1, 1, f);
                    }
                    fclose(f);
                }

                logic.part_fit = '\0';
                logic.part_start = 0;
                strcpy(logic.part_name, "");
                logic.part_size = 0;
                logic.part_status = '0';

            }
        }else{
            cout<<"Error: debe desmontar la partición para poder eliminarla"<<endl;
        }

    }else{
        cout << "Error: no se encontró la partición: "<<name<<endl;
    }
    //fdVER(path);
}

void actLogica(char path[512], char name[16], ebr logica){
    mbr retorno = leerMBR(path);

    partitiond particiones[4];
    particiones[0] = retorno.mbr_partition_1;
    particiones[1] = retorno.mbr_partition_2;
    particiones[2] = retorno.mbr_partition_3;
    particiones[3] = retorno.mbr_partition_4;

    string tipo="";
    bool hayExt = false;
    int i;
    for (i = 0; i < 4; i++) {
        tipo="";
        tipo=particiones[i].part_type;
        if(tipo=="E"){
            hayExt = true;
            break;
        }
    }

    int inicio = 0;
    int tamano = 0;
    int tamanoDiponible = 0;

    if(hayExt==true) {
        int inicio = particiones[i].part_start;
        int tamano = particiones[i].part_size;
        int tamanoDiponible = inicio + tamano;
        //cout<<"La partición extendida es la "<<(i+1)<<"del disco "<<endl;
        tamanoDiponible = inicio + tamano; //No es el espacio disponible en si, si no el ultimo byte de la partición extendida

        string auxf = path;
        size_t pos = 0;
        string res = "";
        while((pos = auxf.find("/"))!=string::npos){
            res += auxf.substr(0,pos)+"/";
            auxf.erase(0,pos + 1);
        }

        string nombre = "";
        pos = auxf.find(".");
        nombre += auxf.substr(0,pos);
        auxf.erase(0,pos + 1);

        char ruta2[512]="";
        strcpy(ruta2,res.c_str());
        strcat(ruta2,nombre.c_str());
        strcat(ruta2,"_rd.dsk");

        FILE *f;
        if ((f = fopen(path, "r+b")) == NULL) {
            if ((f = fopen(ruta2, "r+b")) == NULL) {
                cout<<"Error: no se ha podido al abrir el disco!\n";
            }else{
                ebr B_ebr;

                fseek(f, inicio, SEEK_SET);
                fread(&B_ebr, sizeof (ebr), 1, f);
                if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                    ebr anterior;
                    if(strcmp(B_ebr.part_name,logica.part_name) == 0){
                        fseek(f, logica.part_start, SEEK_SET);
                        fwrite(&logica, sizeof (ebr), 1, f);
                        return;
                    }
                    anterior = B_ebr;
                    int siguiente = B_ebr.part_next;
                    ebr aux;

                    while (true){
                        fseek(f, siguiente, SEEK_SET);
                        fread(&aux, sizeof (ebr), 1, f);

                        if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente
                            if(strcmp(B_ebr.part_name,logica.part_name) == 0){
                                fseek(f, logica.part_start, SEEK_SET);
                                fwrite(&logica, sizeof (ebr), 1, f);
                                return;
                            }
                            siguiente = aux.part_next;
                            anterior = aux;
                        } else { //Es el ultimo

                            break;
                        }
                    }
                }
                fclose(f);
            }
        }else{
            ebr B_ebr;

            fseek(f, inicio, SEEK_SET);
            fread(&B_ebr, sizeof (ebr), 1, f);
            if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                ebr anterior;
                if(strcmp(B_ebr.part_name,logica.part_name) == 0){
                    fseek(f, logica.part_start, SEEK_SET);
                    fwrite(&logica, sizeof (ebr), 1, f);
                    return;
                }
                anterior = B_ebr;
                int siguiente = B_ebr.part_next;
                ebr aux;

                while (true){
                    fseek(f, siguiente, SEEK_SET);
                    fread(&aux, sizeof (ebr), 1, f);

                    if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente
                        if(strcmp(B_ebr.part_name,logica.part_name) == 0){
                            fseek(f, logica.part_start, SEEK_SET);
                            fwrite(&logica, sizeof (ebr), 1, f);
                            return;
                        }
                        siguiente = aux.part_next;
                        anterior = aux;
                    } else { //Es el ultimo

                        break;
                    }
                }
            }
            fclose(f);
        }
    }else{
        cout<<"Error: No se encontró ninguna partición extendida en el disco"<<endl;
        return;
    }
}

void actualizarLogica(char path[512], ebr logica){
        string auxf = path;
        size_t pos = 0;
        string res = "";
        while((pos = auxf.find("/"))!=string::npos){
            res += auxf.substr(0,pos)+"/";
            auxf.erase(0,pos + 1);
        }

        string nombre = "";
        pos = auxf.find(".");
        nombre += auxf.substr(0,pos);
        auxf.erase(0,pos + 1);

        char ruta2[512]="";
        strcpy(ruta2,res.c_str());
        strcat(ruta2,nombre.c_str());
        strcat(ruta2,"_rd.dsk");

        FILE *f;
        if ((f = fopen(path, "r+b")) == NULL) {

        }else{
            ebr B_ebr;
            fseek(f, logica.part_start, SEEK_SET);
            fwrite(&logica, sizeof (logica), 1, f);
        }
        if ((f = fopen(ruta2, "r+b")) == NULL) {

        }else{
            ebr B_ebr;

            fseek(f, logica.part_start, SEEK_SET);
            fwrite(&logica, sizeof (logica), 1, f);
        }
        fclose(f);
}

prtLogica buscarLogica(char path[512], char name[64], partitiond particiones[4], int i) {
    int inicio = particiones[i].part_start;
    int tamano = particiones[i].part_size;
    int tamanoDiponible = inicio + tamano;

    string auxf = path;
    size_t pos = 0;
    string res = "";
    while ((pos = auxf.find("/")) != string::npos) {
        res += auxf.substr(0, pos) + "/";
        auxf.erase(0, pos + 1);
    }

    string nombre = "";
    pos = auxf.find(".");
    nombre += auxf.substr(0, pos);
    auxf.erase(0, pos + 1);

    char ruta2[512] = "";
    strcpy(ruta2, res.c_str());
    strcat(ruta2, nombre.c_str());
    strcat(ruta2, "_rd.dsk");

    FILE *f;
    if ((f = fopen(path, "r+b")) == NULL) {
        if ((f = fopen(ruta2, "r+b")) == NULL) {
            cout << "Error: no se ha podido al abrir el disco!\n";
        } else {
            ebr B_ebr;
            prtLogica rtrnLogica;
            rtrnLogica.encontrado = false;

            fseek(f, inicio, SEEK_SET);
            fread(&B_ebr, sizeof(ebr), 1, f);
            if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                char nombre1[16] = "";
                char nombre2[16] = "";

                strcpy(nombre1, B_ebr.part_name);
                strcpy(nombre2, name);

                if (strcmp(nombre2, nombre1) == 0) {
                    rtrnLogica.B_ebr = B_ebr;
                    rtrnLogica.encontrado = true;
                    return rtrnLogica;
                }

                ebr anterior;
                anterior = B_ebr;
                int siguiente = B_ebr.part_next;
                ebr aux;

                while (true) {
                    fseek(f, siguiente, SEEK_SET);
                    fread(&aux, sizeof(ebr), 1, f);

                    if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente

                        strcpy(nombre1, aux.part_name);
                        strcpy(nombre2, name);
                        if (strncmp(nombre2, nombre1, 16) == 0) {
                            rtrnLogica.B_ebr = aux;
                            rtrnLogica.encontrado = true;
                            return rtrnLogica;
                        }

                        siguiente = aux.part_next;
                        anterior = aux;
                    } else {
                        break;
                    }
                }
            }
            fclose(f);
            return rtrnLogica;
        }
    } else {
        ebr B_ebr;
        prtLogica rtrnLogica;
        rtrnLogica.encontrado = false;

        fseek(f, inicio, SEEK_SET);
        fread(&B_ebr, sizeof(ebr), 1, f);
        if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
            char nombre1[16] = "";
            char nombre2[16] = "";

            strcpy(nombre1, B_ebr.part_name);
            strcpy(nombre2, name);

            if (strcmp(nombre2, nombre1) == 0) {
                rtrnLogica.B_ebr = B_ebr;
                rtrnLogica.encontrado = true;
                return rtrnLogica;
            }

            ebr anterior;
            anterior = B_ebr;
            int siguiente = B_ebr.part_next;
            ebr aux;

            while (true) {
                fseek(f, siguiente, SEEK_SET);
                fread(&aux, sizeof(ebr), 1, f);

                if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente

                    strcpy(nombre1, aux.part_name);
                    strcpy(nombre2, name);
                    if (strncmp(nombre2, nombre1, 16) == 0) {
                        rtrnLogica.B_ebr = aux;
                        rtrnLogica.encontrado = true;
                        return rtrnLogica;
                    }

                    siguiente = aux.part_next;
                    anterior = aux;
                } else {
                    break;
                }
            }
        }
        fclose(f);
        return rtrnLogica;
    }
}

void cFdisk_add(char unit[16], char path[512], char name[64], int add) {
    //Evaluando unit
    string str_unit="";
    str_unit+=unit;
    if(str_unit!="") {
        if (str_unit != "B" && str_unit != "b" && str_unit != "K" && str_unit != "k" && str_unit != "M" && str_unit != "m") {
            cout << "Error: El parámetro \"@unit\" solo recibe los valores: B, K y M" << endl;
            return;
        }
    }else{
        strcpy(unit,"K");
    }

    str_unit="";
    str_unit+=unit;
    if(str_unit=="B"||str_unit=="b"){

    }else if(str_unit=="K"||str_unit=="k"){
        add = 1024 * add;
    }else if(str_unit=="M"||str_unit=="m"){
        add = 1024 * 1024 * add;
    }

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

    int prtActiva[] = {0,0,0,0};

    for (int j = 0; j < 4; j++) {
        if(particiones[j].part_size!=0){
            prtActiva[j]=1;
        }
    }

    int tamanoDisponible = 0;
    if(encontrado==true){
        if(i==0){ //Casos en los que se puede añadir a la partición 1
            //Añadiendo a la partición 1 del disco
            cout<<"Añadiendo a la partición 1 del disco"<<endl;
            if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==0){//1 0 0 0
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[0].part_start+particiones[0].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_size<<"bytes"<<endl;
                    particiones[0].part_size = particiones[0].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==1){//1 0 0 1
                tamanoDisponible = particiones[3].part_start - (particiones[0].part_start+particiones[0].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_size<<"bytes"<<endl;
                    particiones[0].part_size = particiones[0].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==0){//1 0 1 0
                tamanoDisponible = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_size<<"bytes"<<endl;
                    particiones[0].part_size = particiones[0].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==1){//1 0 1 1
                tamanoDisponible = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_size<<"bytes"<<endl;
                    particiones[0].part_size = particiones[0].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0){//1 1 0 0
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_size<<"bytes"<<endl;
                    particiones[0].part_size = particiones[0].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==1){//1 1 0 1
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_size<<"bytes"<<endl;
                    particiones[0].part_size = particiones[0].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0){//1 1 1 0
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_size+1);
                cout<<add<<"El tamaño disponible es: "<<tamanoDisponible<<endl;
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_size<<"bytes"<<endl;
                    particiones[0].part_size = particiones[0].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1){//1 1 1 1
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_size<<"bytes"<<endl;
                    particiones[0].part_size = particiones[0].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_size<<"bytes"<<endl;

                }
            }
        } else if(i==1){ //Casos en los que se puede añadir a la partición 2
            cout<<"Añadiendo a la partición 2 del disco"<<endl;
            if(prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0){//X 1 0 0
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[1].part_start+particiones[1].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[1].part_size<<"bytes"<<endl;
                    particiones[1].part_size = particiones[1].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[1].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==1){//X 1 0 1
                tamanoDisponible = particiones[3].part_start - (particiones[1].part_start+particiones[1].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[1].part_size<<"bytes"<<endl;
                    particiones[1].part_size = particiones[1].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[1].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0){//X 1 1 0
                tamanoDisponible = particiones[2].part_start - (particiones[1].part_start+particiones[1].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[1].part_size<<"bytes"<<endl;
                    particiones[1].part_size = particiones[1].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[1].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1){//X 1 1 1
                tamanoDisponible = particiones[2].part_start - (particiones[1].part_start+particiones[1].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[1].part_size<<"bytes"<<endl;
                    particiones[1].part_size = particiones[1].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[1].part_size<<"bytes"<<endl;

                }
            }
        } else if(i==2){ //Casos en los que se puede añadir a la partición 3
            cout<<"Añadiendo a la partición 3 del disco"<<endl;
            if(prtActiva[2]==1 && prtActiva[3]==0){//X X 1 0
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[2].part_start+particiones[2].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[2].part_size<<"bytes"<<endl;
                    particiones[2].part_size = particiones[2].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[2].part_size<<"bytes"<<endl;

                }
            }else if(prtActiva[2]==1 && prtActiva[3]==1){//X X 1 1
                tamanoDisponible = particiones[3].part_start - (particiones[2].part_start+particiones[2].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[2].part_size<<"bytes"<<endl;
                    particiones[2].part_size = particiones[2].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[2].part_size<<"bytes"<<endl;

                }
            }
        } else if(i==3){ //Casos en los que se puede añadir a la partición 4
            cout<<"Añadiendo a la partición 4 del disco"<<endl;
            if(prtActiva[3]==1){//X X 1 1
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[3].part_start+particiones[3].part_size+1);
                if(add>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[3].part_size<<"bytes"<<endl;

                    particiones[3].part_size = particiones[3].part_size + add;
                    cout<<"Tamaño nuevo: "<<particiones[3].part_size<<"bytes"<<endl;
                }
            }
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

        actualizarMBR(B_mbr,path);
    }else{
        cout << "Error: no se encontró la partición: "<<name<<endl;
    }

    //fdVER(path);
}

void cFdisk_mover(char path[512], char name[64], char mov[64]){
    cout<<"En terminos de @mov, no tenemos @mov :'("<<endl;
}

void fdVER(char path[512]){
    mbr retorno = leerMBR(path);

    partitiond particiones[4];
    particiones[0] = retorno.mbr_partition_1;
    particiones[1] = retorno.mbr_partition_2;
    particiones[2] = retorno.mbr_partition_3;
    particiones[3] = retorno.mbr_partition_4;

    for (int i = 0; i < 4; ++i) {
        cout<<"********** Partición "<<(i+1)<<" **********"<<endl;
        cout<<"Name: "<<particiones[i].part_name<<endl;
        cout<<"Status: "<<particiones[i].part_status<<endl;
        cout<<"Tipo: "<<particiones[i].part_type<<endl;
        cout<<"Inicio: "<<particiones[i].part_start<<endl;
        cout<<"FIN: "<<(particiones[i].part_start+particiones[i].part_size)<<endl;

        if(particiones[i].part_type=='E'){
            int inicio = particiones[i].part_start;
            int tamano = particiones[i].part_size;
            int tamanoDiponible = inicio + tamano;

            string auxf = path;
            size_t pos = 0;
            string res = "";
            while((pos = auxf.find("/"))!=string::npos){
                res += auxf.substr(0,pos)+"/";
                auxf.erase(0,pos + 1);
            }

            string nombre = "";
            pos = auxf.find(".");
            nombre += auxf.substr(0,pos);
            auxf.erase(0,pos + 1);

            char ruta2[512]="";
            strcpy(ruta2,res.c_str());
            strcat(ruta2,nombre.c_str());
            strcat(ruta2,"_rd.dsk");

            FILE *f;
            if ((f = fopen(path, "r+b")) == NULL) {
                if ((f = fopen(ruta2, "r+b")) == NULL) {
                    cout<<"Error: no se ha podido al abrir el disco!\n";
                }else{
                    ebr B_ebr;

                    fseek(f, inicio, SEEK_SET);
                    fread(&B_ebr, sizeof (ebr), 1, f);
                    if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                        cout<<"************************"<<endl;
                        cout<<"Name: "<<B_ebr.part_name<<endl;
                        cout<<"Status: "<<B_ebr.part_status<<endl;
                        cout<<"Inicio: "<<B_ebr.part_start<<endl;
                        cout<<"FIN: "<<(B_ebr.part_start+B_ebr.part_size)<<endl;

                        ebr anterior;
                        anterior = B_ebr;
                        int siguiente = B_ebr.part_next;
                        ebr aux;

                        while (true){
                            fseek(f, siguiente, SEEK_SET);
                            fread(&aux, sizeof (ebr), 1, f);

                            if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente
                                cout<<"************************"<<endl;
                                cout<<"Name: "<<aux.part_name<<endl;
                                cout<<"Status: "<<aux.part_status<<endl;
                                cout<<"Inicio: "<<aux.part_start<<endl;
                                cout<<"FIN: "<<(aux.part_start+aux.part_size)<<endl;

                                siguiente = aux.part_next;
                                anterior = aux;
                            }else{
                                break;
                            }
                        }
                    }
                    fclose(f);
                }
            }else{
                ebr B_ebr;

                fseek(f, inicio, SEEK_SET);
                fread(&B_ebr, sizeof (ebr), 1, f);
                if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                    cout<<"************************"<<endl;
                    cout<<"Name: "<<B_ebr.part_name<<endl;
                    cout<<"Status: "<<B_ebr.part_status<<endl;
                    cout<<"Inicio: "<<B_ebr.part_start<<endl;
                    cout<<"FIN: "<<(B_ebr.part_start+B_ebr.part_size)<<endl;

                    ebr anterior;
                    anterior = B_ebr;
                    int siguiente = B_ebr.part_next;
                    ebr aux;

                    while (true){
                        fseek(f, siguiente, SEEK_SET);
                        fread(&aux, sizeof (ebr), 1, f);

                        if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente
                            cout<<"************************"<<endl;
                            cout<<"Name: "<<aux.part_name<<endl;
                            cout<<"Status: "<<aux.part_status<<endl;
                            cout<<"Inicio: "<<aux.part_start<<endl;
                            cout<<"FIN: "<<(aux.part_start+aux.part_size)<<endl;

                            siguiente = aux.part_next;
                            anterior = aux;
                        }else{
                            break;
                        }
                    }
                }
                fclose(f);
            }
        }

    }
    cout<<"*******************************************"<<endl;
}