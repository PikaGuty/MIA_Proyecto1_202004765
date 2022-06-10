#include <iostream>

#include "Analizador.h"
#include "cMount.h"

using namespace std;

int main() {
    inicializarListaMount();
    cout <<"********************************************************************* " << endl;
    cout <<"********** Javier Alejandro Gutierrez de Leon - 202004765 *********** " << endl;
    cout <<"********************************************************************* " << endl;
    while (true){
        analizar();
    }

    return 0;
}
