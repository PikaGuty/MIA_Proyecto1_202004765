//
// Created by javierg on 3/06/22.
//

#include "Analizador.h"
#include <iostream>
#include <string.h>
#include <string>



#include <algorithm>

using namespace std;

void comando_MKDISK(char *token);

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
            cout<<"******** Ejecutando MKDISK ********"<<endl;
            token = strtok(NULL, " ");
            comando_MKDISK(token);
        }

        token = strtok(NULL, " ");
    }
}

void comando_MKDISK(char *token){
    bool xsize = false, xpath = false, xname = false;
    int size;
    char path[512], name[64];

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
            size = stoi(comandos.substr(0, comandos.find(" ")));

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
    }else{ //Notificando errores si no se ingresaron los parametros obligatorios al comando
        cout<<"El comando \"MKDISK\" debe poseer el/los parámetros ";
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