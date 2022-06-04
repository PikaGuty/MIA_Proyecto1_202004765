//
// Created by javierg on 3/06/22.
//

#include "Analizador.h"
#include <iostream>
#include <string.h>



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
            cout<<"Efectivamente es mkdisk"<<endl;
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

    while (token != NULL) {
        string comando="";
        comando += token;

        /*string ncomando = comando.substr(0, comando.find("=>"));
        transform(ncomando.begin(), ncomando.end(), ncomando.begin(), ::tolower);

        int pos = comando.find("=>");
        if(ncomando == "$size" && xsize == false){
            comando.erase(0, 2+pos);
            cout<<"SIZE ES "<< comando.substr(0, comando.find("=>"))<<endl;
        }else if(ncomando == "$path" && xpath == false){
            comando.erase(0, 2+pos);
            cout<<"PATH ES "<< comando.substr(0, comando.find("=>"))<<endl;
        }else if(ncomando == "$name" && xname == false){
            comando.erase(0, 2+pos);
            cout<<"Name ES "<< comando.substr(0, comando.find("=>"))<<endl;
        }*/
        cout<<"COMANDO "<<token<<endl;

        token = strtok(NULL, " ");
    }
}