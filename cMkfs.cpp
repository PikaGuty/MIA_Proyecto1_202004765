//
// Created by javierg on 11/06/22.
//

#include "cMkfs.h"

void fechActual(times fecha);

using namespace std;

void cMkfs(int add, char id[16], char unitt[16], char type[16]){

    char unit;
    string uni = unitt;
    if (uni == " ")//por si no viene unidad
        add = add * 1024;
    else{
        if (uni == "B" || uni == "b")
            add = add;
        else if (uni == "K" || uni == "k")
            add = add * 1024;
        else if (uni == "M" || uni == "m")
            add = add * 1024 * 1024;
    }


    if (strncmp(type, "", 16) == 0)
        strcpy(type, "full");

    if (add != 0) {
        //TODO agregar o restar
        cout<<"Agregar o restar"<<endl;
    } else {//formatear normal
        mnt_nodo particion = retornarNodoMount(id);
        if (strncmp(particion.mnt_ruta, "", 512) == 0)
            return;

        int part_inicio = 0;
        int part_tamano = 0;
        char part_colocacion = ' ';
        int pimerEspacioEBR = 0;

        char path[512];
        char nombre[16];
        char status;

        if (particion.mnt_particion.part_fit == 'B' || particion.mnt_particion.part_fit == 'F' || particion.mnt_particion.part_fit == 'W') {//es primaria
            part_inicio = particion.mnt_particion.part_start;
            part_tamano = particion.mnt_particion.part_size;
            part_colocacion = particion.mnt_particion.part_type;
            strcpy(path,particion.mnt_ruta);
            strcpy(nombre,particion.mnt_particion.part_name);
            status = particion.mnt_particion.part_status;
            pimerEspacioEBR = 0;
        } else {//Si es el ebr
            part_inicio = particion.mnt_ebr.part_start;
            part_tamano = particion.mnt_ebr.part_size;
            part_colocacion = metodoDeColocacionExtendida;
            strcpy(path,particion.mnt_ruta);
            strcpy(nombre,particion.mnt_ebr.part_name);
            status = particion.mnt_ebr.part_status;
            pimerEspacioEBR = sizeof (ebr);
        }


            double partta = (double) part_tamano;
            double nu;
            nu = (partta - sizeof(superBloque)) / (4.0 + 3.0 * 64.0 + sizeof(inodo) + sizeof(journalie));
            cout << "SuperBloque = " << sizeof(superBloque) << "| Inodo = " << sizeof(inodo) << "|Journalie = "
                 << sizeof(journalie) << endl;
            cout << "Tamaño de la partición = " << part_tamano << endl;
            cout << "N en double = " << nu << endl;
            int n = (int) nu;
            cout << "N en entero = " << n << endl;
            int disk = sizeof(superBloque) + n + n * sizeof(journalie) + 3 * n + n * sizeof(inodo) +
                       3 * n * sizeof(bloqueCarpeta);
            cout << "Tamaño de formato es = " << disk << endl;

            if (status != '2') {
                crear_ext3(particion, n, part_inicio + pimerEspacioEBR); //creando los sectores, super bloque, inodos

                actualizarStatus(path, nombre, '2');
                crearRoot(id);
                cout << "\t...................Se ha formateado la partición................" << endl;
            }

    }
}

void crear_ext3(mnt_nodo mountNodo, int n, int inicioParticion) {
    //cout<<"EL INICIO DE LA PARTICION"<<inicioParticion<<endl;
    //cout<<"Tamaño del EBR "<< sizeof(ebr)<<endl;
    superBloque sb = sb_inicializar(n, mountNodo.tiempo, inicioParticion);
    //cout<<"Antes de escribir "<<sb.s_inodes_count<<endl;
    sb_escribir(mountNodo.mnt_ruta, inicioParticion, sb); //Escribiendo el super bloque

    journalie jr[3*n]; //arreglo de journalie
    jr_escribir(inicioParticion + sizeof (superBloque), n, mountNodo.mnt_ruta, jr); //Journaling

    //Bitmap de bloques
    bmBloque agrregloBmb[n * 3];
    bmBloque bmb;
    bmb.status = '0';
    int u;
    for (u = 0; u < 3 * n; u++) {
        agrregloBmb[u] = bmb;
    }
    agrregloBmb[0].status='1';
    bmb_escribir(sb.s_bm_block_start, n*3, mountNodo.mnt_ruta, agrregloBmb);

    //Bit map de inodos
    bmInodo bmi[n];
    bmInodo bm;
    bm.status = '0';
    int l;
    for (l = 0; l < n; l++) {
        bmi[l] = bm;
    }
    bmi[0].status='1';
    bmi_escribir(sb.s_bm_inode_start, n, mountNodo.mnt_ruta, bmi);


    // inodos
    inodo agrregloInodo[n];
    inodo ino;

    times fech;
    fechActual(fech);
    ino.i_uid = 1;
    ino.i_gid = 1;
    ino.i_size = 0;
    strcpy(ino.i_atime,"");
    strcpy(ino.i_ctime,fech);
    strcpy(ino.i_mtime,"");
    for (int j = 0; j < 15; ++j) {
        ino.i_block[j]=-1;
    }
    ino.i_type='0';

    for (int t = 0; t < n; t++) {
        agrregloInodo[t] = ino;
    }

    inodos_escribir(sb.s_inode_start, n, mountNodo.mnt_ruta, agrregloInodo);


}

superBloque sb_inicializar(int n, times tiempo, int inicio) {//inicializo las variables del superbloque

    //cout<<"iniInicio en = "<<inicio<<endl;
    superBloque sb;

    sb.s_inodes_count = n;
    sb.s_blocks_count = 3 * n;

    sb.s_free_blocks_counts = 3 * n;
    sb.s_free_inodes_count = n;

    strcpy(sb.s_mtime, tiempo);
    strcpy(sb.s_unmtime, tiempo);

    sb.s_mnt_count = 1;
    int v2 = rand() % 666 + 1;
    sb.s_magic = v2;

    sb.s_inode_size = sizeof (inodo);
    sb.s_block_size = sizeof (bloqueArchivo);

    sb.s_first_ino = inicio + sizeof (superBloque) + n * sizeof (journalie) + 3 * n + n; //esta es la primer posicion y ya se agrego el +1 //primer inodo libre
    sb.s_first_blo = sb.s_first_ino + n * sizeof (inodo); //primer bloque libre

    sb.s_bm_inode_start = inicio + sizeof (superBloque) + n * sizeof (journalie)+1;
    sb.s_bm_block_start = sb.s_bm_inode_start + n+1;

    sb.s_inode_start = inicio + sizeof (superBloque) + n * sizeof (journalie) + 3 * n + n;
    sb.s_block_start = sb.s_inode_start + n * sizeof (inodo);

    return sb;
}
void sb_escribir(char ruta[512], int inicio, superBloque sb) {
    string auxf = ruta;
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

    bool pudo1 = true, pudo2 = true;

    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        pudo2=false;
    } else {

        fseek(f, inicio, SEEK_SET);
        fwrite(&sb, sizeof (superBloque), 1, f);
        fclose(f);
    }

    if ((f = fopen(ruta2, "r+b")) == NULL) {
        pudo1=false;
    } else {

        fseek(f, inicio, SEEK_SET);
        fwrite(&sb, sizeof (superBloque), 1, f);
        fclose(f);
    }

    if(pudo1==false && pudo2==false){
        cout<<"Error: no se pudo abrir el disco!"<<endl;
    }
}
superBloque sb_retornar(char id[16]) {
    superBloque sb;
    mnt_nodo particion = retornarNodoMount(id);

    if(strcmp(particion.mnt_ruta,"")==0){
        sb.s_inodes_count=-1;
        return sb;
    }

    ////////los datos necesarios
    int part_inicio = 0;
    int part_tamano = 0;
    char part_colocacion = ' ';

    int tamanoEBR = 0;
    if (particion.mnt_particion.part_fit == 'B' || particion.mnt_particion.part_fit == 'F' || particion.mnt_particion.part_fit == 'W') {//es primaria
        part_inicio = particion.mnt_particion.part_start;
        part_tamano = particion.mnt_particion.part_size;
        part_colocacion = particion.mnt_particion.part_type;
        tamanoEBR = 0;
    } else {//del ebr
        part_inicio = particion.mnt_ebr.part_start;
        part_tamano = particion.mnt_ebr.part_size;
        part_colocacion = metodoDeColocacionExtendida;
        tamanoEBR = sizeof (ebr);
    }
    //hay que abirir el archivo

    string auxf = particion.mnt_ruta;
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

    bool pudo1 = true, pudo2 = true;

    FILE *f;
    if ((f = fopen(particion.mnt_ruta, "r+b")) == NULL) {
        if ((f = fopen(ruta2, "r+b")) == NULL) {
            cout<<"Error: no se pudo abrir el disco!"<<endl;
        } else {

            fseek(f, part_inicio + tamanoEBR, SEEK_SET);
            fread(&sb, sizeof (superBloque), 1, f);
            fclose(f);
        }
    } else {

        fseek(f, part_inicio + tamanoEBR, SEEK_SET);
        //cout<<"INICIO "<<part_inicio+tamanoEBR<<endl;
        fread(&sb, sizeof (superBloque), 1, f);
        fclose(f);
    }
    return sb;
}

void jr_escribir(int inicio, int n, char ruta[512], journalie aux[]) {

    string auxf = ruta;
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

    bool pudo1 = true, pudo2 = true;

    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        pudo1= false;
    } else {
        int j;
        //cout<<"Deberia llegar a "<<(3 * n + 1)<<endl;
        //cout<<"El tamaño es "<<sizeof (aux)<<endl;
        for (j = 0; j < n; j++) {
            fseek(f, inicio + j * (sizeof (journalie)), SEEK_SET);
            //cout<<"Posicionado en "<<(inicio + j * (sizeof (journalie)))<<endl;
            //cout<<"Va por "<<j<<" "<<(inicio + j * (sizeof (journalie)))<<endl;
            fwrite(&aux[j], sizeof (journalie), 1, f);
        }

        fclose(f);
    }

    if ((f = fopen(ruta2, "r+b")) == NULL) {
        pudo2=false;
    } else {
        int j;
        for (j = 0; j < n; j++) {
            fseek(f, inicio + j * (sizeof (journalie)), SEEK_SET);
            fwrite(&aux[j], sizeof (journalie), 1, f);
        }
        fclose(f);
    }

    if(pudo2==false && pudo1==false){
        cout<<"Error: no se pudo abrir el disco!"<<endl;
    }
}

void bmb_escribir(int inicio, int n, char ruta[512], bmBloque aux[]) {
    string auxf = ruta;
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

    bool pudo1 = true, pudo2 = true;

    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        pudo1=false;
    } else {
        int j;
        for (j = 0; j < n; j++) {
            fseek(f, inicio + j * (sizeof (bmBloque)), SEEK_SET);
            fwrite(&aux[j], sizeof (bmBloque), 1, f);
        }
        fclose(f);
    }
    if ((f = fopen(ruta2, "r+b")) == NULL) {
        pudo2=false;
    } else {
        int j;
        for (j = 0; j < n; j++) {
            fseek(f, inicio + j * (sizeof (bmBloque)), SEEK_SET);
            fwrite(&aux[j], sizeof (bmBloque), 1, f);
        }
        fclose(f);
    }

    if(pudo1==false && pudo2==false){
        cout<<"Error: no se pudo abrir el disco!"<<endl;
    }
}
void bmb_leer(int inicio, int n, char ruta[512], bmBloque *aux) {
    string auxf = ruta;
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

    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        if ((f = fopen(ruta2, "r+b")) == NULL) {
            cout<<"Error: no se pudo abrir el disco!"<<endl;
        } else {
            int j;
            for (j = 0; j < n ; j++) {
                fseek(f, inicio + j * (sizeof (bmBloque)), SEEK_SET);
                fread(&aux[j], sizeof (bmBloque), 1, f);
            }
            fclose(f);
        }
    } else {
        int j;
        for (j = 0; j < n; j++) {
            fseek(f, inicio + j * (sizeof (bmBloque)), SEEK_SET);
            fread(&aux[j], sizeof (bmBloque), 1, f);
        }
        fclose(f);
    }
}

void bmi_escribir(int inicio, int n, char ruta[512], bmInodo aux[]) {
    string auxf = ruta;
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

    bool pudo1 = true;
    bool pudo2 = true;
    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        pudo1 = false;
    } else {
        int j;
        for (j = 0; j < n; j++) {
            fseek(f, inicio + j * (sizeof (bmInodo)), SEEK_SET);
            //bmInodo aux2 = aux[j];
            fwrite(&aux[j], sizeof (bmInodo), 1, f);
        }
        fclose(f);
    }
    if ((f = fopen(ruta2, "r+b")) == NULL) {
        pudo2 = false;
    } else {
        int j;
        for (j = 0; j < n; j++) {
            fseek(f, inicio + j * (sizeof (bmInodo)), SEEK_SET);
            //bmInodo aux2 = aux[j];
            fwrite(&aux[j], sizeof (bmInodo), 1, f);
        }
        fclose(f);
    }

    if(pudo1==false && pudo2==false){
        cout<<"Error: no se pudo abrir el disco!"<<endl;
    }
}
void bmi_leer(int inicio, int n, char ruta[512], bmInodo *aux) {
    string auxf = ruta;
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

    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        if ((f = fopen(ruta2, "r+b")) == NULL) {
            cout<<"Error: no se pudo abrir el disco!"<<endl;
        } else {
            int j;
            for (j = 0; j < n; j++) {
                //bmInodo aux2 = aux[j];
                fseek(f, inicio + j * (sizeof (bmInodo)), SEEK_SET);
                fread(&aux[j], sizeof (bmInodo), 1, f);
            }
            fclose(f);
        }
    } else {
        int j;
        for (j = 0; j < n; j++) {
            //bmInodo aux2 = aux[j];
            fseek(f, inicio + j * (sizeof (bmInodo)), SEEK_SET);
            fread(&aux[j], sizeof (bmInodo), 1, f);
        }
        fclose(f);
    }
}

void inodos_escribir(int inicio, int n, char ruta[512], inodo aux[]) {
    string auxf = ruta;
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

    bool pudo1 = true, pudo2 = true;
    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        pudo1 = false;
    } else {
        int j;
        for (j = 0; j < n; j++) {
            fseek(f, inicio + j * (sizeof (inodo)), SEEK_SET);
            fwrite(&aux[j], sizeof (inodo), 1, f);
        }
        fclose(f);
    }
    if ((f = fopen(ruta2, "r+b")) == NULL) {
        pudo2 = false;
    } else {
        int j;
        for (j = 0; j < n; j++) {
            fseek(f, inicio + j * (sizeof (inodo)), SEEK_SET);
            fwrite(&aux[j], sizeof (inodo), 1, f);
        }
        fclose(f);
    }

    if(pudo1==false && pudo2==false){
        cout<<"Error: no se pudo abrir el disco!"<<endl;
    }

}
void inodos_leer(int inicio, int n, char ruta[512], inodo *aux) {
    string auxf = ruta;
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

    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        if ((f = fopen(ruta2, "r+b")) == NULL) {
            cout<<"Error: no se pudo abrir el disco!"<<endl;
        } else {
            int j;
            for (j = 0; j < n; j++) {
                fseek(f, inicio + j * (sizeof (inodo)), SEEK_SET);
                fread(&aux[j], sizeof (inodo), 1, f);
            }
            fclose(f);
        }
    } else {
        int j;
        for (j = 0; j < n; j++) {
            fseek(f, inicio + j * (sizeof (inodo)), SEEK_SET);
            fread(&aux[j], sizeof (inodo), 1, f);
        }
        fclose(f);
    }
}
inodo inodos_leer1(int inicio, int n, char ruta[512], inodo aux) {
    string auxf = ruta;
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

    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        if ((f = fopen(ruta2, "r+b")) == NULL) {
            cout<<"Error: no se pudo abrir el disco!"<<endl;
        } else {
            fseek(f, inicio, SEEK_SET);
            fread(&aux, sizeof (inodo), 1, f);

            fclose(f);
        }
    } else {
        fseek(f, inicio, SEEK_SET);
        fread(&aux, sizeof (inodo), 1, f);

        fclose(f);
    }
    return aux;
}
inodo inodos_escribir1(int inicio, int n, char ruta[512], inodo aux) {
    string auxf = ruta;
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

    bool pos1=true, pos2=true;
    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        pos1=false;
    } else {
        fseek(f, inicio, SEEK_SET);
        fwrite(&aux, sizeof (inodo), 1, f);

        fclose(f);
    }
    if ((f = fopen(ruta2, "r+b")) == NULL) {
        pos2= false;
    } else {
        fseek(f, inicio, SEEK_SET);
        fwrite(&aux, sizeof (inodo), 1, f);

        fclose(f);
    }
    if(pos1 == false && pos2==false){
        cout<<"Error: no se pudo abrir el disco!"<<endl;
    }
    return aux;
}

//Estos son para meter solo 1
void blocksC_escribir(int inicio, int n, char ruta[512], bloqueCarpeta aux) {
    string auxf = ruta;
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

    bool pudo1 = true, pudo2 = true;
    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        pudo1 = false;
    } else {
        fseek(f, inicio, SEEK_SET);
        fwrite(&aux, sizeof (bloqueCarpeta), 1, f);

        fclose(f);
    }
    if ((f = fopen(ruta2, "r+b")) == NULL) {
        pudo2 = false;
    } else {
        fseek(f, inicio, SEEK_SET);
        fwrite(&aux, sizeof (bloqueCarpeta), 1, f);

        fclose(f);
    }

    if(pudo1==false && pudo2==false){
        cout<<"Error: no se pudo abrir el disco!"<<endl;
    }

}
bloqueCarpeta blocksC_leer(int inicio, int n, char ruta[512], bloqueCarpeta aux) {
    string auxf = ruta;
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

    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        if ((f = fopen(ruta2, "r+b")) == NULL) {
            cout<<"Error: no se pudo abrir el disco!"<<endl;
        } else {
            fseek(f, inicio, SEEK_SET);
            fread(&aux, sizeof (bloqueCarpeta), 1, f);

            fclose(f);
        }
    } else {
        fseek(f, inicio, SEEK_SET);
        fread(&aux, sizeof (bloqueCarpeta), 1, f);

        fclose(f);
    }
    return aux;
}

void blocksA_escribir(int inicio, int n, char ruta[512], bloqueArchivo aux) {
    string auxf = ruta;
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

    bool pudo1 = true, pudo2 = true;
    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        pudo1 = false;
    } else {
        fseek(f, inicio, SEEK_SET);
        fwrite(&aux, sizeof (bloqueArchivo), 1, f);

        fclose(f);
    }
    if ((f = fopen(ruta2, "r+b")) == NULL) {
        pudo2 = false;
    } else {
        fseek(f, inicio, SEEK_SET);
        fwrite(&aux, sizeof (bloqueArchivo), 1, f);

        fclose(f);
    }

    if(pudo1==false && pudo2==false){
        cout<<"Error: no se pudo abrir el disco!"<<endl;
    }

}
bloqueArchivo blocksA_leer(int inicio, int n, char ruta[512], bloqueArchivo aux) {
    string auxf = ruta;
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

    bool pudo1 = true, pudo2 = true;
    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        if ((f = fopen(ruta2, "r+b")) == NULL) {
            cout<<"Error: no se pudo abrir el disco!"<<endl;
        } else {
            fseek(f, inicio, SEEK_SET);
            fread(&aux, sizeof (bloqueArchivo), 1, f);

            fclose(f);
        }
    } else {
        fseek(f, inicio, SEEK_SET);
        fread(&aux, sizeof (bloqueArchivo), 1, f);

        fclose(f);
    }
    return aux;

}

void blocksAp_escribir(int inicio, int n, char ruta[512], bloqueApuntadores aux) {
    string auxf = ruta;
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

    bool pudo1 = true, pudo2 = true;
    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        pudo1 = false;
    } else {
        fseek(f, inicio, SEEK_SET);
        fwrite(&aux, sizeof (bloqueApuntadores), 1, f);

        fclose(f);
    }
    if ((f = fopen(ruta2, "r+b")) == NULL) {
        pudo2 = false;
    } else {
        fseek(f, inicio, SEEK_SET);
        fwrite(&aux, sizeof (bloqueApuntadores), 1, f);

        fclose(f);
    }

    if(pudo1==false && pudo2==false){
        cout<<"Error: no se pudo abrir el disco!"<<endl;
    }

}
bloqueApuntadores blocksAp_leer(int inicio, int n, char ruta[512], bloqueApuntadores aux) {
    string auxf = ruta;
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

    bool pudo1 = true, pudo2 = true;
    FILE *f;
    if ((f = fopen(ruta, "r+b")) == NULL) {
        if ((f = fopen(ruta2, "r+b")) == NULL) {
            cout<<"Error: no se pudo abrir el disco!"<<endl;
        } else {
            fseek(f, inicio, SEEK_SET);
            fread(&aux, sizeof (bloqueApuntadores), 1, f);

            fclose(f);
        }
    } else {
        fseek(f, inicio, SEEK_SET);
        fread(&aux, sizeof (bloqueApuntadores), 1, f);

        fclose(f);
    }
    return aux;

}

void fechActual(times fecha) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    strcpy(fecha,buf);
}