//
// Created by javierg on 12/06/22.
//

#include "cRep.h"

string rtfecha(time_t now) ;
bool escribirDOT(string dot, char pathCe[512], char pathC[512], char extension[6]);


void rMBR(char path[512], char nombre[16], char extension[6], char id[64]);
void rDISK(char path[512], char nombre[16], char extension[6], char id[16]);
void rSB(char path[512], char nombre[16], char extension[6], char id[16]);

void reportes(char path[512], char namee[64], char id[64]){
    string nam = namee;
    transform(nam.begin(), nam.end(), nam.begin(), ::tolower);

    string auxf = path;
    size_t pos = 0;
    string res = "";
    while ((pos = auxf.find("/")) != string::npos) {
        res += auxf.substr(0, pos) + "/";
        auxf.erase(0, pos + 1);
    }
    char ruta[512];
    strcpy(ruta,res.c_str());

    string nombree = "";
    pos = auxf.find(".");
    nombree += auxf.substr(0, pos);
    auxf.erase(0, pos + 1);
    char nombre[16];
    strcpy(nombre,nombree.c_str());
    char extension[6];
    strcpy(extension,auxf.c_str());

    /*cout<<"Inicio path: "<<res<<endl;
    cout<<"Nombre: "<<nombree<<endl;
    cout<<"Extension: "<<auxf<<endl;*/

    string crear = "sudo -S mkdir -p \'"+res+"\'";
    system(crear.c_str());
    //cout<<aux<<endl;
    string compermiso = "sudo -S chmod -R 777 \'"+res+"\'";
    system(compermiso.c_str());

    if(nam=="mbr"){
        rMBR(ruta, nombre, extension, id);
    }else if(nam=="disk"){
        rDISK(ruta, nombre, extension, id);
    }else if(nam=="block"){
        //TODO reporte block
    }else if(nam=="bm_block"){
        //TODO reporte block
    }else if(nam=="bm_inode"){
        //TODO reporte bminode
    }else if(nam=="inode"){
        //TODO reporte inode
    }else if(nam=="journaling"){
        //TODO reporte journaling
    }else if(nam=="tree"){
        //TODO reporte tree
    }else if(nam=="sb"){
        rSB(ruta, nombre, extension, id);
    }else if(nam=="file"){
        //TODO reporte file
    }else{
        cout<<"Error: "<<namee<<" no es un reporte"<<endl;
    }
}

void rMBR(char path[512], char nombre[16], char extension[6], char id[16]){
    char rutaEscribir[512];
    string dot = "digraph h {\n"
                 "label=<\n"
                 "    <TABLE border=\"3\" bgcolor=\"#60D394\" >\n"
                 "    \n"
                 "    <TR><TD border=\"2\"  bgcolor=\"#A4036F\" gradientangle=\"315\" colspan=\"2\" >REPORTE MBR</TD></TR>\n"
                 "    \n"
                 "    <TR><TD border=\"2\"  bgcolor=\"#EE6055\" gradientangle=\"315\" colspan=\"2\" >MBR</TD></TR>\n";
    strcpy(rutaEscribir,path);
    strcat(rutaEscribir,nombre);
    strcat(rutaEscribir,".");
    strcat(rutaEscribir,extension);

    mnt_nodo particion = retornarNodoMount(id);

    //cout<<"Vamos a buscar en "<<particion.mnt_ruta<<endl;
    mbr retorno = leerMBR(particion.mnt_ruta);

    dot += "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"  gradientangle=\"315\">Tamaño</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot+= to_string(retorno.mbr_tamano);
    dot+="</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"  gradientangle=\"315\">Fecha de Creación</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    string fechaa= rtfecha(retorno.mbr_fecha_creacion);
    dot+= fechaa;
    dot+="</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Signature</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot+= to_string(retorno.mbr_dsk_signature);
    dot+="</TD>\n"
           "    </TR>\n";

    partitiond particiones[4];
    particiones[0] = retorno.mbr_partition_1;
    particiones[1] = retorno.mbr_partition_2;
    particiones[2] = retorno.mbr_partition_3;
    particiones[3] = retorno.mbr_partition_4;

    for (int i = 0; i < 4; ++i) {
        dot += "<TR><TD border=\"2\"  bgcolor=\"#EE6055\" gradientangle=\"315\" colspan=\"2\" >Partición ";
        dot += to_string(i+1);
        dot += "</TD></TR>\n"
               "    \n"
               "    <TR>\n"
               "    <TD border=\"1\"  bgcolor=\"#127ABB\"  gradientangle=\"315\">Status</TD>\n"
               "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
        dot += particiones[i].part_status;
        dot +="</TD>\n"
               "    </TR>\n"
               "    \n"
               "    <TR>\n"
               "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Name</TD>\n"
               "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
        dot += particiones[i].part_name;
        dot +="</TD>\n"
               "    </TR>\n"
               "    \n"
               "    <TR>\n"
               "    <TD border=\"1\"  bgcolor=\"#127ABB\"  gradientangle=\"315\">Type</TD>\n"
               "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
        dot += particiones[i].part_type;
        dot +="</TD>\n"
               "    </TR>\n"
               "    \n"
               "    <TR>\n"
               "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Fit</TD>\n"
               "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
        dot +=particiones[i].part_fit;
        dot +="</TD>\n"
               "    </TR>\n"
               "    \n"
               "    <TR>\n"
               "    <TD border=\"1\"  bgcolor=\"#127ABB\"  gradientangle=\"315\">Start</TD>\n"
               "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
        dot += to_string(particiones[i].part_start);
        dot +="</TD>\n"
               "    </TR>\n"
               "    \n"
               "    <TR>\n"
               "    <TD border=\"1\"  bgcolor=\"#127ABB\"  gradientangle=\"315\">Size</TD>\n"
               "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
        dot += to_string(particiones[i].part_size);
        dot +="</TD>\n"
               "    </TR>\n";


        if(particiones[i].part_type=='E'){
            int inicio = particiones[i].part_start;
            int tamano = particiones[i].part_size;
            int tamanoDiponible = inicio + tamano;

            string auxf = particion.mnt_ruta;
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
            if ((f = fopen(particion.mnt_ruta, "r+b")) == NULL) {
                if ((f = fopen(ruta2, "r+b")) == NULL) {
                    cout<<"Error: no se ha podido al abrir el disco!\n";
                }else{
                    ebr B_ebr;

                    fseek(f, inicio, SEEK_SET);
                    fread(&B_ebr, sizeof (ebr), 1, f);
                    if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                        dot += " <TR><TD border=\"2\"  bgcolor=\"#F0F757\" gradientangle=\"315\" colspan=\"2\" >Partición lógica</TD></TR>\n"
                               "    \n"
                               "    <TR>\n"
                               "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Status</TD>\n"
                               "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                        dot+=B_ebr.part_status;
                        dot+="</TD>\n"
                               "    </TR>\n"
                               "    \n"
                               "    <TR>\n"
                               "    <TD border=\"1\"  bgcolor=\"#CC5A71\"   gradientangle=\"315\">Name</TD>\n"
                               "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                        dot+=B_ebr.part_name;
                        dot+="</TD>\n"
                               "    </TR>\n"
                               "    \n"
                               "    <TR>\n"
                               "    <TD border=\"1\"  bgcolor=\"#CC5A71\"   gradientangle=\"315\">Fit</TD>\n"
                               "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                        dot+=B_ebr.part_fit;
                        dot+="</TD>\n"
                               "    </TR>\n"
                               "    \n"
                               "    <TR>\n"
                               "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Start</TD>\n"
                               "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                        dot+= to_string(B_ebr.part_start);
                        dot+="</TD>\n"
                               "    </TR>\n"
                               "    \n"
                               "    <TR>\n"
                               "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Size</TD>\n"
                               "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                        dot+= to_string(B_ebr.part_size);
                        dot+="</TD>\n"
                               "    </TR>";


                        ebr anterior;
                        anterior = B_ebr;
                        int siguiente = B_ebr.part_next;
                        ebr aux;

                        while (true){
                            fseek(f, siguiente, SEEK_SET);
                            fread(&aux, sizeof (ebr), 1, f);

                            if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente
                                dot += " <TR><TD border=\"2\"  bgcolor=\"#F0F757\" gradientangle=\"315\" colspan=\"2\" >Partición lógica</TD></TR>\n"
                                       "    \n"
                                       "    <TR>\n"
                                       "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Status</TD>\n"
                                       "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                                dot+=aux.part_status;
                                dot+="</TD>\n"
                                     "    </TR>\n"
                                     "    \n"
                                     "    <TR>\n"
                                     "    <TD border=\"1\"  bgcolor=\"#CC5A71\"   gradientangle=\"315\">Name</TD>\n"
                                     "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                                dot+=aux.part_name;
                                dot+="</TD>\n"
                                     "    </TR>\n"
                                     "    \n"
                                     "    <TR>\n"
                                     "    <TD border=\"1\"  bgcolor=\"#CC5A71\"   gradientangle=\"315\">Fit</TD>\n"
                                     "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                                dot+=aux.part_fit;
                                dot+="</TD>\n"
                                     "    </TR>\n"
                                     "    \n"
                                     "    <TR>\n"
                                     "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Start</TD>\n"
                                     "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                                dot+= to_string(aux.part_start);
                                dot+="</TD>\n"
                                     "    </TR>\n"
                                     "    \n"
                                     "    <TR>\n"
                                     "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Size</TD>\n"
                                     "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                                dot+= to_string(aux.part_size);
                                dot+="</TD>\n"
                                     "    </TR>";

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
                    dot += " <TR><TD border=\"2\"  bgcolor=\"#F0F757\" gradientangle=\"315\" colspan=\"2\" >Partición lógica</TD></TR>\n"
                           "    \n"
                           "    <TR>\n"
                           "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Status</TD>\n"
                           "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";

                    dot+=B_ebr.part_status;
                    dot+="</TD>\n"
                         "    </TR>\n"
                         "    \n"
                         "    <TR>\n"
                         "    <TD border=\"1\"  bgcolor=\"#CC5A71\"   gradientangle=\"315\">Name</TD>\n"
                         "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                    dot+=B_ebr.part_name;
                    dot+="</TD>\n"
                         "    </TR>\n"
                         "    \n"
                         "    <TR>\n"
                         "    <TD border=\"1\"  bgcolor=\"#CC5A71\"   gradientangle=\"315\">Fit</TD>\n"
                         "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                    dot+=B_ebr.part_fit;
                    dot+="</TD>\n"
                         "    </TR>\n"
                         "    \n"
                         "    <TR>\n"
                         "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Start</TD>\n"
                         "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                    dot+= to_string(B_ebr.part_start);
                    dot+="</TD>\n"
                         "    </TR>\n"
                         "    \n"
                         "    <TR>\n"
                         "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Size</TD>\n"
                         "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                    dot+= to_string(B_ebr.part_size);
                    dot+="</TD>\n"
                         "    </TR>";


                    ebr anterior;
                    anterior = B_ebr;
                    int siguiente = B_ebr.part_next;
                    ebr aux;

                    while (true){
                        fseek(f, siguiente, SEEK_SET);
                        fread(&aux, sizeof (ebr), 1, f);

                        if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente
                            dot += " <TR><TD border=\"2\"  bgcolor=\"#F0F757\" gradientangle=\"315\" colspan=\"2\" >Partición lógica</TD></TR>\n"
                                   "    \n"
                                   "    <TR>\n"
                                   "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Status</TD>\n"
                                   "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                            dot+=aux.part_status;
                            dot+="</TD>\n"
                                 "    </TR>\n"
                                 "    \n"
                                 "    <TR>\n"
                                 "    <TD border=\"1\"  bgcolor=\"#CC5A71\"   gradientangle=\"315\">Name</TD>\n"
                                 "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                            dot+=aux.part_name;
                            dot+="</TD>\n"
                                 "    </TR>\n"
                                 "    \n"
                                 "    <TR>\n"
                                 "    <TD border=\"1\"  bgcolor=\"#CC5A71\"   gradientangle=\"315\">Fit</TD>\n"
                                 "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                            dot+=aux.part_fit;
                            dot+="</TD>\n"
                                 "    </TR>\n"
                                 "    \n"
                                 "    <TR>\n"
                                 "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Start</TD>\n"
                                 "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                            dot+= to_string(aux.part_start);
                            dot+="</TD>\n"
                                 "    </TR>\n"
                                 "    \n"
                                 "    <TR>\n"
                                 "    <TD border=\"1\"  bgcolor=\"#CC5A71\"  gradientangle=\"315\">Size</TD>\n"
                                 "    <TD border=\"1\"  bgcolor=\"#C89B7B\"  gradientangle=\"315\">";
                            dot+= to_string(aux.part_size);
                            dot+="</TD>\n"
                                 "    </TR>";

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
    dot+="</TABLE>>\n"
         "    \n"
         "    }";
    //cout<<dot<<endl;

    char rutaCe[512],rutaC[512];
    strcpy(rutaCe,path);
    strcat(rutaCe,nombre);
    strcat(rutaCe,".");
    strcpy(rutaC,rutaCe);
    strcat(rutaCe,extension);
    escribirDOT(dot,rutaCe,rutaC,extension);
}

void rDISK(char path[512], char nombre[16], char extension[6], char id[16]){
    char rutaEscribir[512];
    string dot = "digraph D {\n"
                 "\n"
                 "    node [fontname=\"Arial\"];\n"
                 "\n"
                 "    node_A [shape=record label=\"";
    strcpy(rutaEscribir,path);
    strcat(rutaEscribir,nombre);
    strcat(rutaEscribir,".");
    strcat(rutaEscribir,extension);

    mnt_nodo particion = retornarNodoMount(id);

    //cout<<"Vamos a buscar en "<<particion.mnt_ruta<<endl;
    mbr retorno = leerMBR(particion.mnt_ruta);

    dot += "    MBR\\nTamaño: ";
    dot+= to_string(retorno.mbr_tamano);
    dot += "\\n";
    dot+="Fecha de Creación: ";
    string fechaa= rtfecha(retorno.mbr_fecha_creacion);
    dot+= fechaa;
    dot += "\\n";
    dot+="Signature: ";
    dot+= to_string(retorno.mbr_dsk_signature);


    partitiond particiones[4];
    particiones[0] = retorno.mbr_partition_1;
    particiones[1] = retorno.mbr_partition_2;
    particiones[2] = retorno.mbr_partition_3;
    particiones[3] = retorno.mbr_partition_4;

    for (int i = 0; i < 4; ++i) {
        dot += "|";
        if(particiones[i].part_type=='E') {
            dot += "{Name: ";
            dot += particiones[i].part_name;
            dot += "\\n";
            dot += "Type: ";
            dot += particiones[i].part_type;
            dot += "\\n";
            dot += "Size: ";
            dot += to_string(particiones[i].part_size);
            dot += "\\n";
            if (particiones[i].part_status == '2') {
                dot += "Bloques total: ";
                dot += to_string(666);
                dot += "\\n";
                dot += "Bloques ocupados: ";
                dot += to_string(10);
                dot += "\\n";
            }
            dot += "|";
        }else{
            dot += "Name: ";
            dot += particiones[i].part_name;
            dot += "\\n";
            dot += "Type: ";
            dot += particiones[i].part_type;
            dot += "\\n";
            dot += "Size: ";
            dot += to_string(particiones[i].part_size);
            dot += "\\n";
            if (particiones[i].part_status == '2') {
                dot += "Bloques total: ";
                dot += to_string(666);
                dot += "\\n";
                dot += "Bloques ocupados: ";
                dot += to_string(10);
                dot += "\\n";
            }
        }


        if(particiones[i].part_type=='E'){
            int inicio = particiones[i].part_start;
            int tamano = particiones[i].part_size;
            int tamanoDiponible = inicio + tamano;

            string auxf = particion.mnt_ruta;
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
            if ((f = fopen(particion.mnt_ruta, "r+b")) == NULL) {
                if ((f = fopen(ruta2, "r+b")) == NULL) {
                    cout<<"Error: no se ha podido al abrir el disco!\n";
                }else{
                    ebr B_ebr;

                    fseek(f, inicio, SEEK_SET);
                    fread(&B_ebr, sizeof (ebr), 1, f);
                    if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                        dot += "{Name: ";
                        dot+=B_ebr.part_name;
                        dot += "\\n";
                        dot += "Tipo: L";
                        dot += "\\n";
                        dot+="Size: ";
                        dot+= to_string(B_ebr.part_size);
                        dot += "\\n";
                        if (B_ebr.part_status == '2') {
                            dot += "Bloques total: ";
                            dot += to_string(666);
                            dot += "\\n";
                            dot += "Bloques ocupados: ";
                            dot += to_string(10);
                            dot += "\\n";
                        }


                        ebr anterior;
                        anterior = B_ebr;
                        int siguiente = B_ebr.part_next;
                        ebr aux;

                        while (true){
                            fseek(f, siguiente, SEEK_SET);
                            fread(&aux, sizeof (ebr), 1, f);

                            if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente
                                dot += "|Name: ";
                                dot+=aux.part_name;
                                dot += "\\n";
                                dot += "Tipo: L";
                                dot += "\\n";
                                dot+="Size: ";
                                dot+= to_string(aux.part_size);
                                dot += "\\n";
                                if (aux.part_status == '2') {
                                    dot += "Bloques total: ";
                                    dot += to_string(666);
                                    dot += "\\n";
                                    dot += "Bloques ocupados: ";
                                    dot += to_string(10);
                                    dot += "\\n";
                                }

                                siguiente = aux.part_next;
                                anterior = aux;
                            }else{
                                break;
                            }
                        }
                    }
                    dot += "}";
                    fclose(f);
                }
            }else{
                ebr B_ebr;

                fseek(f, inicio, SEEK_SET);
                fread(&B_ebr, sizeof (ebr), 1, f);
                if (B_ebr.part_fit == 'B' || B_ebr.part_fit == 'F' || B_ebr.part_fit == 'W') {
                    dot += "{Name: ";
                    dot+=B_ebr.part_name;
                    dot += "\\n";
                    dot += "Tipo: L";
                    dot += "\\n";
                    dot+="Size: ";
                    dot+= to_string(B_ebr.part_size);
                    dot += "\\n";
                    if (B_ebr.part_status == '2') {
                        dot += "Bloques total: ";
                        dot += to_string(666);
                        dot += "\\n";
                        dot += "Bloques ocupados: ";
                        dot += to_string(10);
                        dot += "\\n";
                    }


                    ebr anterior;
                    anterior = B_ebr;
                    int siguiente = B_ebr.part_next;
                    ebr aux;

                    while (true){
                        fseek(f, siguiente, SEEK_SET);
                        fread(&aux, sizeof (ebr), 1, f);

                        if (aux.part_fit == 'B' || aux.part_fit == 'F' || aux.part_fit == 'W') { //Hay siguiente
                            dot += "|Name: ";
                            dot+=aux.part_name;
                            dot += "\\n";
                            dot += "Tipo: L";
                            dot += "\\n";
                            dot+="Size: ";
                            dot+= to_string(aux.part_size);
                            dot += "\\n";
                            if (aux.part_status == '2') {
                                dot += "Bloques total: ";
                                dot += to_string(666);
                                dot += "\\n";
                                dot += "Bloques ocupados: ";
                                dot += to_string(10);
                                dot += "\\n";
                            }

                            siguiente = aux.part_next;
                            anterior = aux;
                        }else{
                            break;
                        }
                    }
                }
                dot += "}}";
                fclose(f);
            }
        }

    }
    dot+="\"];\n"
         "\n"
         "}";
    cout<<dot<<endl;

    char rutaCe[512],rutaC[512];
    strcpy(rutaCe,path);
    strcat(rutaCe,nombre);
    strcat(rutaCe,".");
    strcpy(rutaC,rutaCe);
    strcat(rutaCe,extension);
    escribirDOT(dot,rutaCe,rutaC,extension);
}

void rSB(char path[512], char nombre[16], char extension[6], char id[16]){
    superBloque sb = sb_retornar(id);
    string dot = "digraph G { \n"
                 "\n"
                 "    label=<\n"
                 "    <TABLE border=\"3\" bgcolor=\"#60D394\" >\n"
                 "    \n"
                 "\n"
                 "    \n"
                 "    <TR><TD border=\"2\"  bgcolor=\"#EE6055\" gradientangle=\"315\" colspan=\"2\" >Super Bloque";
    dot += id;
    dot += "</TD></TR>\n"
           "      \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"  gradientangle=\"315\">Número total de inodos</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_inodes_count);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"  gradientangle=\"315\">Número total de bloques</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_blocks_count);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Número de bloques libres</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_free_inodes_count);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"  gradientangle=\"315\">Número de inodos libres</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_free_blocks_counts);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Fecha en el que el sistema fue montado</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += sb.s_mtime;
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Fecha en que el sistema fue desmontado</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += (sb.s_unmtime);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Número de veces se ha montado el sistema</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_mnt_count);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Valor que identifica al sistema de archivos</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_magic);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Tamaño del inodo</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_inode_size);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Tamaño del bloque</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_block_size);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Primer inodo libre</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_first_ino);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Primer bloque libre</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_first_blo);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Inicio del bitmap de inodos</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_bm_inode_start);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Inicio del bitmap de bloques</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_bm_block_start);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Inicio de la tabla de inodos</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_inode_start);
    dot += "</TD>\n"
           "    </TR>\n"
           "    \n"
           "    <TR>\n"
           "    <TD border=\"1\"  bgcolor=\"#127ABB\"   gradientangle=\"315\">Inico de la tabla de bloques</TD>\n"
           "    <TD border=\"1\"  bgcolor=\"#F0D7B6\"  gradientangle=\"315\">";
    dot += to_string(sb.s_block_start);
    dot += "</TD>\n"
           "    </TR>\n"
           "\n"
           "    </TABLE>>\n"
           "    \n"
           "    }";


    char rutaCe[512],rutaC[512];
    strcpy(rutaCe,path);
    strcat(rutaCe,nombre);
    strcat(rutaCe,".");
    strcpy(rutaC,rutaCe);
    strcat(rutaCe,extension);
    escribirDOT(dot,rutaCe,rutaC,extension);
}

bool escribirDOT(string dot, char pathCe[512], char pathC[512], char extension[6]){
    cout<<dot<<endl;
    string ext = extension;
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    char pathDOT[512];
    strcpy(pathDOT,pathC);
    strcat(pathDOT,"dot");

    //cout<<"Voy a escribir en "<<pathDOT<<endl;
    ofstream myfile;
    myfile.open (pathDOT);
    myfile << dot;
    myfile.close();

    char cambio[512];

    char comando[1024];
    strcpy(comando,"dot");
    strcat(comando," \'");
    strcat(comando,pathDOT);
    strcat(comando,"\' -T");
    strcat(comando,ext.c_str());
    strcat(comando," -o \'");
    strcat(comando,pathCe);
    strcat(comando,"\'");
    system(comando);
    cout<<comando<<endl;
    cout<<"\t...................Se ha creado el reporte................"<<endl;

}

string rtfecha(time_t now) {
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), " %d-%m-%Y %X", &tstruct);
    string fech = buf;

    return fech;
}