//
// Created by javierg on 12/06/22.
//

#include "cRep.h"

string rtfecha(time_t now) ;

void rMBR(char path[512], char nombre[16], char extension[6], char id[64]);

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
        //TODO reporte disk
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
        //TODO reporte sb
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
    cout<<dot<<endl;


}

string rtfecha(time_t now) {
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), " %d-%m-%Y %X", &tstruct);
    string fech = buf;

    return fech;
}