//
// Created by javierg on 3/06/22.
//

#include "Analizador.h"
#include <iostream>
#include <string.h>
#include <string>
#include <algorithm>


using namespace std;

void comando_MKDISK(char *token); //YA
void comando_RMDISK(char *token); //YA
void comando_FDISK(char *token); //Quitar los VER()
void comando_MOUNT(char *token); //YA
void comando_UNMOUNT(char *token); //YA
void comando_MKFS(char *token);
void comando_MKFILE(char *token);
void comando_EXEC(char *token);
void comando_RM(char *token);
void comando_MKDIR(char *token);
void comando_CP(char *token);
void comando_LS(char *token);
void comando_PAUSE(char *token);
void comando_LOSS(char *token);
void comando_REP(char *token);

void analizar(){
    cout <<endl<<"INGRESE UN COMANDO VALIDO: " << endl;
    string texto;
    getline(cin,texto);//OBTENIENDO TEXTOcin.getline(curs.nombre,16,'\n');

    //CONVERTIR A ARREGLO DE CHAR
    char str[1024];
    strcpy(str, texto.c_str());

    //SEPARANDO
    char *token = strtok(str, " ");

    while (token != NULL) {
        string actual = "";
        actual+=token;
        transform(actual.begin(), actual.end(), actual.begin(), ::tolower);

        if (actual=="mkdisk"){
            cout<<"************************ Ejecutando MKDISK ************************"<<endl;
            token = strtok(NULL, " ");
            comando_MKDISK(token);
        }else if (actual=="rmdisk"){
            cout<<"************************ Ejecutando RMDISK ************************"<<endl;
            token = strtok(NULL, " ");
            comando_RMDISK(token);
        }else if (actual=="fdisk"){
            cout<<"************************ Ejecutando FDISK ************************"<<endl;
            token = strtok(NULL, " ");
            comando_FDISK(token);
        }else if (actual=="mount"){
            cout<<"************************ Ejecutando MOUNT ************************"<<endl;
            token = strtok(NULL, " ");
            comando_MOUNT(token);
        }else if (actual=="unmount"){
            cout<<"************************ Ejecutando UNMOUNT ************************"<<endl;
            token = strtok(NULL, " ");
            comando_UNMOUNT(token);
        }else if (actual=="mkfs"){
            cout<<"************************ Ejecutando MKFS ************************"<<endl;
            token = strtok(NULL, " ");
            comando_MKFS(token);
        }else if (actual=="mkfile"){
            cout<<"************************ Ejecutando MKFILE ************************"<<endl;
            token = strtok(NULL, " ");
            comando_MKFILE(token);
        }else if (actual=="exec"){
            cout<<"************************ Ejecutando EXEC ************************"<<endl;
            token = strtok(NULL, " ");
            comando_EXEC(token);
        }else if (actual=="rm"){
            cout<<"************************ Ejecutando RM ************************"<<endl;
            token = strtok(NULL, " ");
            comando_RM(token);
        }else if (actual=="mkdir"){
            cout<<"************************ Ejecutando MKDIR ************************"<<endl;
            token = strtok(NULL, " ");
            comando_MKDIR(token);
        }else if (actual=="cp"){ // TODO preguntar que tabla usar
            cout<<"************************ Ejecutando CP ************************"<<endl;
            token = strtok(NULL, " ");
            comando_CP(token);
        }/*else if (actual=="ls"){ TODO preguntar que tabla usar
            cout<<"************************ Ejecutando LS ************************"<<endl;
            token = strtok(NULL, " ");
            comando_LS(token);
        }*/else if (actual=="pause"){
            cout<<"************************ Ejecutando PAUSE ************************"<<endl;
            token = strtok(NULL, " ");
            comando_PAUSE(token);
        }else if (actual=="loss"){
            cout<<"************************ Ejecutando LOSS ************************"<<endl;
            token = strtok(NULL, " ");
            comando_LOSS(token);
        }else if (actual=="rep"){
            cout<<"************************ Ejecutando REP ************************"<<endl;
            token = strtok(NULL, " ");
            comando_REP(token);
        }else {
            cout << "Error: Este comando no existe" << endl;
        }


        token = strtok(NULL, " ");
    }
}

void analizarEX(string texto){

    //CONVERTIR A ARREGLO DE CHAR
    char str[1024];
    strcpy(str, texto.c_str());

    string s1="/*",s2="*/";
    if (texto.find(s1) != std::string::npos && texto.find(s2) != std::string::npos) {
        cout<<texto<<endl;
    }else {
        if(texto!="") {
            cout << "\n*** COMANDO: " << texto << " ***" << endl;
        }
        //SEPARANDO
        char *token = strtok(str, " ");

        while (token != NULL) {
            string actual = "";
            actual += token;
            transform(actual.begin(), actual.end(), actual.begin(), ::tolower);

            if (actual == "mkdisk") {
                cout << "************************ Ejecutando MKDISK ************************" << endl;
                token = strtok(NULL, " ");
                comando_MKDISK(token);
            } else if (actual == "rmdisk") {
                cout << "************************ Ejecutando RMDISK ************************" << endl;
                token = strtok(NULL, " ");
                comando_RMDISK(token);
            } else if (actual == "fdisk") {
                cout << "************************ Ejecutando FDISK ************************" << endl;
                token = strtok(NULL, " ");
                comando_FDISK(token);
            } else if (actual == "mount") {
                cout << "************************ Ejecutando MOUNT ************************" << endl;
                token = strtok(NULL, " ");
                comando_MOUNT(token);
            } else if (actual == "unmount") {
                cout << "************************ Ejecutando UNMOUNT ************************" << endl;
                token = strtok(NULL, " ");
                comando_UNMOUNT(token);
            } else if (actual == "mkfs") {
                cout << "************************ Ejecutando MKFS ************************" << endl;
                token = strtok(NULL, " ");
                comando_MKFS(token);
            } else if (actual == "mkfile") {
                cout << "************************ Ejecutando MKFILE ************************" << endl;
                token = strtok(NULL, " ");
                comando_MKFILE(token);
            } else if (actual == "exec") {
                cout << "************************ Ejecutando EXEC ************************" << endl;
                token = strtok(NULL, " ");
                comando_EXEC(token);
            } else if (actual == "rm") {
                cout << "************************ Ejecutando RM ************************" << endl;
                token = strtok(NULL, " ");
                comando_RM(token);
            } else if (actual == "mkdir") {
                cout << "************************ Ejecutando MKDIR ************************" << endl;
                token = strtok(NULL, " ");
                comando_MKDIR(token);
            } else if (actual == "cp") { // TODO preguntar que tabla usar
                cout << "************************ Ejecutando CP ************************" << endl;
                token = strtok(NULL, " ");
                comando_CP(token);
            }/*else if (actual=="ls"){ TODO preguntar que tabla usar
            cout<<"************************ Ejecutando LS ************************"<<endl;
            token = strtok(NULL, " ");
            comando_LS(token);
        }*/else if (actual == "pause") {
                cout << "************************ Ejecutando PAUSE ************************" << endl;
                token = strtok(NULL, " ");
                comando_PAUSE(token);
            } else if (actual == "loss") {
                cout << "************************ Ejecutando LOSS ************************" << endl;
                token = strtok(NULL, " ");
                comando_LOSS(token);
            } else if (actual == "rep") {
                cout << "************************ Ejecutando REP ************************" << endl;
                token = strtok(NULL, " ");
                comando_REP(token);
            } else {
                cout << "Error: Este comando no existe" << endl;
            }


            token = strtok(NULL, " ");
        }
    }
    if(texto!="") {
        cout << "\n" << endl;
    }
}

void comando_MKDISK(char *token){
    bool xsize = false, xpath = false, xname = false;
    int size;
    char path[512], name[64];

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"MKDISK\" debe poseer los parámetros $size, $name, $path ya que son de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        if (ncomando == "$size" && xsize == false) { //Reconociendo el size y sacando su parámetro
            xsize = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE SIZE EL DATO
            try {
                size = stoi(comandos.substr(0, comandos.find(" ")));
            } catch (...) {
                cout<<"Error: El parámetro \"$size\" solo recibe numeros"<<endl;
                return;
            }


            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$path" && xpath == false) {
            xpath = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            //PASANDO A LA VARIABLE PATH EL DATO
            strcpy(path, comandos.substr(0, comandos.find("\"")).c_str());

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$name" && xname == false) {
            xname = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE NAME EL DATO
            strcpy(name,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"MKDISK\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xsize == true && xname == true && xpath==true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"Size: "<<size<<endl;
        cout<<"Name: "<<name<<endl;
        cout<<"Path: "<<path<<endl;
        cout<<endl;
        cMkdisk(size,name,path);

    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"MKDISK\" debe poseer el/los parámetros ";
        if(xsize == false){
            cout<<"$size";
            if(xname == false || xpath == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xname == false){
            cout<<"$name";
            if(xpath == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xpath == false){
            cout<<"$path ";
        }

        cout<<"ya que son de caracter obligatorio."<<endl;
    }
}

void comando_RMDISK(char *token){
    bool xpath = false;
    char path[512];

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"RMDISK\" debe poseer el parámetro $path ya que es de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        if (ncomando == "$path" && xpath == false) {//Reconociendo el path y obteniendo su valor
            xpath = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            //PASANDO A LA VARIABLE PATH EL DATO
            strcpy(path, comandos.substr(0, comandos.find("\"")).c_str());

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else{
            if (comandos!=anterior) {
                cout << "Error: " << ncomando << " no es un parámetro del comando \"RMDISK\"" << endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xpath==true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"Path: "<<path<<endl;
        cout<<endl;
        execRMDISK(path);
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"RMDISK\" debe poseer el parámetro ";

        if(xpath == false){
            cout<<"$path ";
        }

        cout<<"ya que es de caracter obligatorio."<<endl;
    }
}

void comando_FDISK(char *token){
    bool xsize = false, xpath = false, xname = false;//Obligatorios
    bool xunit = false, xtype = false, xfit = false, xdelete = false, xadd = false, xmov = false;//Opcionales
    int size=0, add=0;
    char path[512]="", name[64]="", unit[16]="", type[16]="", fit[16]="", delet[16]="", mov[64]="";

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"FDISK\" debe poseer los parámetros $size, $name, $path ya que son de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        if (ncomando == "$size" && xsize == false) { //Reconociendo el size y sacando su parámetro
            xsize = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE SIZE EL DATO
            try {
                size = stoi(comandos.substr(0, comandos.find(" ")));
            } catch (...) {
                cout<<"Error: El parámetro \"$size\" solo recibe numeros"<<endl;
                return;
            }

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$path" && xpath == false) { //Reconociendo el path y sacando su parámetro
            xpath = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            //PASANDO A LA VARIABLE PATH EL DATO
            strcpy(path, comandos.substr(0, comandos.find("\"")).c_str());

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$name" && xname == false) { //Reconociendo el name y sacando su parámetro
            xname = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE NAME EL DATO
            strcpy(name,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "@unit" && xunit == false) { //Reconociendo el unit y sacando su parámetro
            xunit = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE UNIT EL DATO
            strcpy(unit,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else if (ncomando == "@type" && xtype == false) { //Reconociendo el type y sacando su parámetro
            xtype = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE TYPE EL DATO
            strcpy(type,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else if (ncomando == "@fit" && xfit == false) { //Reconociendo el fit y sacando su parámetro
            xfit = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE FIT EL DATO
            strcpy(fit,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else if (ncomando == "@delete" && xdelete == false) { //Reconociendo el delete y sacando su parámetro
            xdelete = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE DELETE EL DATO
            strcpy(delet,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else if (ncomando == "@add" && xadd == false) { //Reconociendo el add y sacando su parámetro
            xadd = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE ADD EL DATO
            try {
                add = stoi(comandos.substr(0, comandos.find(" ")));
            } catch (...) {
                cout<<"Error: El parámetro \"@add\" solo recibe numeros"<<endl;
                return;
            }

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else if (ncomando == "@mov" && xmov == false) { //Reconociendo el mov y sacando su parámetro
            xmov = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE DELETE EL DATO
            strcpy(mov,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"FDISK\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xname == true && xpath==true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER

        /*cout<<"Size: "<<size<<endl;
        cout<<"Unit: "<<unit<<endl;
        cout<<"Path: "<<path<<endl;
        cout<<"Type: "<<type<<endl;
        cout<<"Fit: "<<fit<<endl;
        cout<<"Delete: "<<delet<<endl;
        cout<<"Name: "<<name<<endl;
        cout<<"Add: "<<add<<endl;
        cout<<"Mov: "<<mov<<endl;
        cout<<endl;*/

        if(xdelete == true){
            //cout<<"Vamo a eliminar"<<endl;
            cFdisk_eliminar(path, delet, name);
        } else if (xadd == true){
            //cout<<"Vamo a añadir"<<endl;
            cFdisk_add(unit, path, name, add);
        } else if (xmov == true){
            //cout<<"Vamo a mover"<<endl;
            cFdisk_mover(path, name, mov);
        }else{
            //cout<<"Vamo a crear"<<endl;
            cFdisk_crear(size, unit, path, type, fit, name);
        }
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"FDISK\" debe poseer el/los parámetros ";
        if(xsize == false){
            cout<<"$size";
            if(xname == false || xpath == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xname == false){
            cout<<"$name";
            if(xpath == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xpath == false){
            cout<<"$path ";
        }

        cout<<"ya que son de caracter obligatorio."<<endl;
    }
}

void comando_MOUNT(char *token){
    bool xpath = false, xname = false;
    char path[512], name[64];

    if(token == NULL){//Error por si no trae ningun parametro
        mntVER();
        return;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        if (ncomando == "$path" && xpath == false) {
            xpath = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            //PASANDO A LA VARIABLE PATH EL DATO
            strcpy(path, comandos.substr(0, comandos.find("\"")).c_str());

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$name" && xname == false) {
            xname = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE NAME EL DATO
            strcpy(name,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"MOUNT\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xname == true && xpath==true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"Name: "<<name<<endl;
        cout<<"Path: "<<path<<endl;
        cMount(path,name);
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"MOUNT\" debe poseer el/los parámetros ";
        if(xname == false){
            cout<<"$name";
            if(xpath == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xpath == false){
            cout<<"$path ";
        }

        cout<<"ya que son de caracter obligatorio."<<endl;
    }
}

void comando_UNMOUNT(char *token){
    bool xid = false;
    string id[32];

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"UNMOUNT\" debe poseer lel parámetro $id ya que es de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior="";
    int cont=0;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        char com[16];
        bool coincide = true;
        strcpy(com, ncomando.c_str());
        for (int i = 0; i < 3; i++) {

                if (i == 0 && com[i] == '$') {
                    //cout << "SI ES $" << endl;
                } else {
                    if(i==0)
                        coincide = false;
                }
                if (i == 1 && com[i] == 'i') {
                    //cout << "SI ES i" << endl;
                } else {
                    if(i==1)
                        coincide = false;
                }
                if (i == 2 && com[i] == 'd') {
                    //cout << "SI ES d" << endl;
                } else {
                    if(i==2)
                        coincide = false;
                }
        }

        if (coincide==true) {
            xid = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE NAME EL DATO
            id[cont]=comandos.substr(0, comandos.find(" "));


            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"UNMOUNT\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
            cont++;
        }
    }


    if(xid == true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"ID: "<<endl;
        for (int i = 0; i < 32; i++) {
            if(id[i]!=""){
                cout<<id[i]<<endl;
            }
        }
        cUnmount(id);
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"UNMOUNT\" debe poseer el/los parámetros ";
       if(xid == false){
            cout<<"$id# ";
        }
        cout<<"ya que son de caracter obligatorio."<<endl;
    }
}

void comando_MKFS(char *token){
    bool xid = false;//Obligatorios
    bool xunit = false, xtype = false, xadd = false;//Opcionales
    int add=0;
    char id[16], unit[16]="", type[16]="";

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"MKFS\" debe poseer el parámetro $id ya que es de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        if (ncomando == "$id" && xid == false) { //Reconociendo el size y sacando su parámetro
            xid = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE ID EL DATO
            strcpy(id,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "@unit" && xunit == false) { //Reconociendo el unit y sacando su parámetro
            xunit = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE UNIT EL DATO
            strcpy(unit,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else if (ncomando == "@type" && xtype == false) { //Reconociendo el type y sacando su parámetro
            xtype = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE TYPE EL DATO
            strcpy(type,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else if (ncomando == "@add" && xadd == false) { //Reconociendo el add y sacando su parámetro
            xadd = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE DELETE EL DATO
            add = stoi(comandos.substr(0, comandos.find(" ")));

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"MKFS\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xid==true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER

        cout<<"Id: "<<id<<endl;
        cout<<"Unit: "<<unit<<endl;
        cout<<"Type: "<<type<<endl;
        cout<<"Add: "<<add<<endl;
        cMkfs(add, id, unit, type);

    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"MKFS\" debe poseer el parámetro ";
        if(xid == false){
            cout<<"$id ";
        }

        cout<<"ya que es de caracter obligatorio."<<endl;
    }
}

void comando_MKFILE(char *token){
    bool xpath = false, xid = false;//Obligatorios
    bool xp = false, xsize = false, xcont = false;//Opcionales
    int size=0, add=0;
    char path[512]="", id[32]="", type[16]="", cont[512]="";

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"MKFILE\" debe poseer los parámetros $id y $path ya que son de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        /*Se puede fragmentar el comando para evaluar si los dos primeros
        caracteres son @p y tomar la decision si hacer o no la linea de abajo o tirarlo a espacio
         en vez de =>*/

        char com[128];
        bool coincide = true;
        strcpy(com, comandos.c_str());
        for (int i = 0; i < 3; i++) {

            if (i == 0 && com[i] == '@') {
                //cout << "SI ES @" << endl;
            } else {
                if (i == 0)
                    coincide = false;
            }
            if (i == 1 && com[i] == 'p') {
                //cout << "SI ES i" << endl;
            } else {
                if (i == 1)
                    coincide = false;
            }
            if (i == 2 && com[i] == ' ') {
                //cout << "SI ES d" << endl;
            } else {
                if (i == 2)
                    coincide = false;
            }
        }


        int pos=0;
        if(coincide== false){
            //cout<<"Evaluando "<<com<<" y es "<<coincide<<endl;
            ncomando = comandos.substr(0, comandos.find("=>"));
            transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
            pos = comandos.find("=>");
        }else{
            ncomando = comandos.substr(0, comandos.find(" "));
            transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
            pos = comandos.find(" ");
        }

        if (ncomando == "$id" && xid == false) { //Reconociendo el size y sacando su parámetro
            xid = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE ID EL DATO
            strcpy(id, comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$path" && xpath == false) { //Reconociendo el path y sacando su parámetro
            xpath = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            //PASANDO A LA VARIABLE PATH EL DATO
            strcpy(path, comandos.substr(0, comandos.find("\"")).c_str());

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "@p" && xp == false) { //Reconociendo el path y sacando su parámetro
            xp = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 1 + pos);
            //Verificar para tirar error

        } else if (ncomando == "@size" && xsize == false) { //Reconociendo el size y sacando su parámetro
            xsize = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE SIZE EL DATO
            size = stoi(comandos.substr(0, comandos.find(" ")));

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "@cont" && xcont == false) { //Reconociendo el path y sacando su parámetro
            xcont = true;//Indicando que ya se evaluó
            comandos.erase(0, 2 + pos);

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            //PASANDO A LA VARIABLE PATH EL DATO
            strcpy(cont, comandos.substr(0, comandos.find("\"")).c_str());

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"MKFILE\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xid == true && xpath==true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"Id: "<<id<<endl;
        cout<<"Path: "<<path<<endl;
        cout<<"P: "<<xp<<endl;
        cout<<"Size: "<<size<<endl;
        cout<<"Cont: "<<cont<<endl;

        cMkfile(path, id, xp, size, cont);
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"MKFILE\" debe poseer el/los parámetros ";

        if(xid == false){
            cout<<"$id";
            if(xid == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xpath == false){
            cout<<"$path ";
        }

        cout<<"ya que son de caracter obligatorio."<<endl;
    }
}

void comando_EXEC(char *token){
    bool xpath = false;//Obligatorios
    char path[512]="";

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"EXEC\" debe poseer los parámetros $size, $name, $path ya que son de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        if (ncomando == "$path" && xpath == false) { //Reconociendo el path y sacando su parámetro
            xpath = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            //PASANDO A LA VARIABLE PATH EL DATO
            strcpy(path, comandos.substr(0, comandos.find("\"")).c_str());

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"EXEC\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xpath==true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"Path: "<<path<<endl;
        execScript(path);
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"EXEC\" debe poseer el parámetro ";

        if(xpath == false){
            cout<<"$path ";
        }

        cout<<"ya que son de caracter obligatorio."<<endl;
    }
}

void comando_RM(char *token){
    bool xid = false, xfileid= false;
    string fileid[32], id;

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"UNMOUNT\" debe poseer lel parámetro $id ya que es de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior="";
    int cont=0;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        if (ncomando == "$id" && xid == false) { //Reconociendo el size y sacando su parámetro
            xid = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE ID EL DATO
            id=comandos.substr(0, comandos.find(" "));

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else if (ncomando=="$fileid") {
            xfileid = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE NAME EL DATO
            fileid[cont]=comandos.substr(0, comandos.find(" "));

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
            cont++;
        }else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"UNMOUNT\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;

        }
    }


    if(xid == true&&xfileid== true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"Fileid: "<<endl;
        for (int i = 0; i < sizeof(id); ++i) {
            if(fileid[i]!=""){
                cout<<fileid[i]<<endl;
            }
        }
        cout<<"ID: "<<id<<endl;
        //TODO RM
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"RM\" debe poseer el/los parámetros ";
        if(xid == false){
            cout<<"$id";
            if(xid == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xfileid == false){
            cout<<"$fileid ";
        }
        cout<<"ya que son de caracter obligatorio."<<endl;
    }
}

void comando_MKDIR(char *token){
    bool xpath = false, xid = false;//Obligatorios
    bool xp = false;//Opcionales
    int add=0;
    char path[512]="", id[16]="";

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"MKDIR\" debe poseer los parámetros $id y $path ya que son de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros

        char com[128];
        bool coincide = true;
        strcpy(com, comandos.c_str());
        for (int i = 0; i < 3; i++) {

            if (i == 0 && com[i] == '@') {
                //cout << "SI ES @" << endl;
            } else {
                if (i == 0)
                    coincide = false;
            }
            if (i == 1 && (com[i] == 'p'||com[i] == 'P')) {
                //cout << "SI ES i" << endl;
            } else {
                if (i == 1)
                    coincide = false;
            }
            if (i == 2 && com[i] == ' ') {
                //cout << "SI ES d" << endl;
            } else {
                if (i == 2)
                    coincide = false;
            }
        }


        int pos=0;
        if(coincide== false){
            ncomando = comandos.substr(0, comandos.find("=>"));
            transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
            pos = comandos.find("=>");
        }else{
            ncomando = comandos.substr(0, comandos.find(" "));
            transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
            pos = comandos.find(" ");
        }

        if (ncomando == "$id" && xid == false) { //Reconociendo el size y sacando su parámetro
            xid = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE ID EL DATO
            strcpy(id, comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$path" && xpath == false) { //Reconociendo el path y sacando su parámetro
            xpath = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            //PASANDO A LA VARIABLE PATH EL DATO
            strcpy(path, comandos.substr(0, comandos.find("\"")).c_str());

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "@p" && xp == false) { //Reconociendo el path y sacando su parámetro
            xp = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 1 + pos);
            //Verificar para tirar error

        } else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"MKDIR\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xid == true && xpath==true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"Id: "<<id<<endl;
        cout<<"Path: "<<path<<endl;
        cout<<"P: "<<xp<<endl;
        cout<<endl;
        cMkdir(path, id, xp);

    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"MKDIR\" debe poseer el/los parámetros ";

        if(xid == false){
            cout<<"$id";
            if(xid == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xpath == false){
            cout<<"$path ";
        }

        cout<<"ya que son de caracter obligatorio."<<endl;
    }
}

void comando_CP(char *token){
    //TODO Preguntar que tabla usar
    bool xid = false, xpath = false, xname = false;
    char path[512], name[64], id[64];

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"CP\" debe poseer los parámetros $id, $name, $path ya que son de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        if (ncomando == "$id" && xid == false) { //Reconociendo el id y sacando su parámetro
            xid = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE SIZE EL DATO
            strcpy(id,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$path" && xpath == false) {
            xpath = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            //PASANDO A LA VARIABLE PATH EL DATO
            strcpy(path, comandos.substr(0, comandos.find("\"")).c_str());

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$name" && xname == false) {
            xname = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE NAME EL DATO
            strcpy(name,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"MKDISK\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xid == true && xname == true && xpath==true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"Id: "<<id<<endl;
        cout<<"Name: "<<name<<endl;
        cout<<"Path: "<<path<<endl;
        //TODO CP
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"CP\" debe poseer el/los parámetros ";
        if(xid == false){
            cout<<"$id";
            if(xname == false || xpath == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xname == false){
            cout<<"$name";
            if(xpath == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xpath == false){
            cout<<"$path ";
        }

        cout<<"ya que son de caracter obligatorio."<<endl;
    }
}

void comando_PAUSE(char *token){
    char id[64];

    if(token == NULL){
        char c;
        cout<<"Presione para continuar"<<endl;
        cin>>c;
    }else{
        cout<<"Error: El comando \"PAUSE\" no debe llevar el parámetros."<<endl;
    }


}

void comando_LOSS(char *token){
    bool xid = false;
    char id[64];

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"LOSS\" debe poseer el parámetro $id ya que es de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        if (ncomando == "$id" && xid == false) { //Reconociendo el id y sacando su parámetro
            xid = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE SIZE EL DATO
            strcpy(id,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"LOSS\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xid == true ){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"Id: "<<id<<endl;
        //TODO LOSS
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"LOSS\" debe poseer el parámetro ";
        if(xid == false){
            cout<<"$id ";
        }

        cout<<"ya que es de caracter obligatorio."<<endl;
    }
}

void comando_REP(char *token){
    bool xid = false, xpath = false, xname = false;
    char path[512], name[64], id[64];

    if(token == NULL){//Error por si no trae ningun parametro
        cout<<"Error: El comando \"REP\" debe poseer los parámetros $id, $name, $path ya que son de caracter obligatorio."<<endl;
    }

    string comandos = "";
    comandos+=token;
    while (token != NULL) { //Convirtiendo los tokens recibidos en una sola cadena
        token = strtok(NULL, " ");
        if (token != NULL){
            comandos+=" ";
            comandos+=token;
        }
    }

    string ncomando, anterior;

    while (comandos != "") { //Volviendo a desarmar la cadena para obtener los parámetros
        ncomando = comandos.substr(0, comandos.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);
        int pos = comandos.find("=>");

        if (ncomando == "$id" && xid == false) { //Reconociendo el id y sacando su parámetro
            xid = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE SIZE EL DATO
            strcpy(id,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$path" && xpath == false) {
            xpath = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            //PASANDO A LA VARIABLE PATH EL DATO
            strcpy(path, comandos.substr(0, comandos.find("\"")).c_str());

            pos = comandos.find("\"");
            comandos.erase(0, 1 + pos);

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        } else if (ncomando == "$name" && xname == false) {
            xname = true;//Indicando que ya se evaluó al ser de caracter obligatorio
            comandos.erase(0, 2 + pos);

            //PASANDO A LA VARIABLE NAME EL DATO
            strcpy(name,comandos.substr(0, comandos.find(" ")).c_str());

            pos = comandos.find(" ");
            comandos.erase(0, 1 + pos);
        }else{
            if (comandos!=anterior){
                cout<<"Error: "<<ncomando<<" no es un parámetro del comando \"REP\""<<endl;
            }
        }

        if (comandos==anterior){ //Verificando si ya se llegó al final (no cambia por que no encuentra espacio)
            comandos="";
        }else{
            anterior=comandos;
        }
    }


    if(xid == true && xname == true && xpath==true){//Se puede ejecutar el comando
        //LLAMAR AL METODO PARA TERMINAR VERIFICACIONES Y REALIZAR LO QUE TIENE QUE HACER
        cout<<"Id: "<<id<<endl;
        cout<<"Name: "<<name<<endl;
        cout<<"Path: "<<path<<endl;
        reportes(path,name,id);
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"Error: El comando \"REP\" debe poseer el/los parámetros ";
        if(xid == false){
            cout<<"$id";
            if(xname == false || xpath == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xname == false){
            cout<<"$name";
            if(xpath == false){
                cout<<", ";
            }else{
                cout<<" ";
            }
        }
        if(xpath == false){
            cout<<"$path ";
        }

        cout<<"ya que son de caracter obligatorio."<<endl;
    }
}

