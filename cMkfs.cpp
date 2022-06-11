//
// Created by javierg on 11/06/22.
//
#include <iostream>
#include "cMkfs.h"
#include <string.h>

using namespace std;

void cMkfs(int add, char id[16], char unitt[16], char type[16]){
    cout<<"Del otro lado"<<endl;
    cout<<"Id: "<<id<<endl;
    cout<<"Unit: "<<unit<<endl;
    cout<<"Type: "<<type<<endl;
    cout<<"Add: "<<add<<endl;

    char *unit;
    strcpy(unit,unitt);

    if (unit == ' ')//por si no viene unidad
        unit = 'k';
    int retorno = 1;
    retorno = strncmp(type, "", 16);
    if (retorno == 0)
        strcpy(type, "full");
}