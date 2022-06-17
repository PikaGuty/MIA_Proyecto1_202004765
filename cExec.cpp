//
// Created by javierg on 16/06/22.
//

#include "cExec.h"
#include <iostream>
#include <string.h>
#include <fstream>

#include "Analizador.h"

using namespace std;

void execScript(char path[512]){
    fstream f;

    f.open(path,ios::in);
    if (f.is_open()){
        string tp;
        while(getline(f, tp)){
            analizarEX(tp);
        }
        f.close();
    }else{
        cout<<"Error: no se ha podido al abrir el archivo!\n";
    }
}