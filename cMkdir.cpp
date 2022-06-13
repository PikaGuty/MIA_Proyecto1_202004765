//
// Created by javierg on 12/06/22.
//

#include "cMkdir.h"

particionMontada devolverParticionMontada(char id[16]);
void fechActual(char fecha[128]);

void cMkdir(char path[512], char id[16], bool p){
    particionMontada m= devolverParticionMontada(id);

    cout<<"------------------ ParticionMontada: Id="<<m.id<<" ------------------"<<endl;
    cout<<"\t\tRuta: "<<m.ruta<<endl;
    cout<<"\t\tNombre: "<<m.part_name<<endl;
    cout<<"\t\tTiempoDeMontaje: "<<m.part_time<<endl;
    cout<<"\t\tTipo = "<< m.part_type<<"\tInicio= "<<m.part_inicio<<"\tTamño= "<<m.part_tamano<<endl;
    cout<<"\t\tColcn= "<<m.part_colocacion<<"\tEspEbr= "<<m.part_espacioEbr<<"\tStatus= "<<m.part_status<<endl;
}

void crearRoot(char id[16]) {
    //primero tengo que obtener el super bloque
    superBloque sb = sb_retornar(id);
    mnt_nodo mountNodo = retornarNodoMount(id); //la particion que tiene los datos
    int n = sb.s_inodes_count;

    //bit map de indos.
    bmInodo bm_Inodos[n];
    bmi_leer(sb.s_bm_inode_start, n, mountNodo.mnt_ruta, bm_Inodos);

    //bit map de bloques
    bmBloque bm_Bloques[3 * n];
    bmb_leer(sb.s_bm_block_start, n, mountNodo.mnt_ruta, bm_Bloques);

    //indos
    inodo Inodos[n];
    inodos_leer(sb.s_inode_start, n, mountNodo.mnt_ruta, Inodos);

    //Modificando los inodos
    Inodos[0].i_size=0;//el tamaño del archivo
    fechActual(Inodos[0].i_atime);//ultima fecha que se leyó el nodo sin modificarlo
    fechActual(Inodos[0].i_ctime);
    fechActual(Inodos[0].i_mtime);
    Inodos[0].i_type=0;
    Inodos[0].i_block[0]= sb.s_first_blo;//primer bloque libre
    inodos_escribir(sb.s_inode_start, n, mountNodo.mnt_ruta, Inodos);//escribiendo lo inodos

    //hay que modificar el super bloque
    sb.s_free_blocks_counts--;
    sb.s_free_inodes_count--;
    sb.s_first_ino=sb.s_first_ino+sizeof(inodo);
    sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta);
    sb.s_bjpurfree=sb.s_bjpurfree+sizeof(journalie);

    //sb_escribir(mountNodo.mnt_ruta,)

}

particionMontada devolverParticionMontada(char id[16]) {
    mnt_nodo mountNodo = retornarNodoMount(id);
    char part_type; //e o p
    int part_inicio; //inicio
    int part_tamano; //tamaño d ela particion
    char part_colocacion; //w,b,
    int part_espacioEbr; //si es extendida
    int part_status = 0; //activa o no
    char part_name[16]; //nombre de la partición.

    char ruta[512]; //ruta del disco

    times part_time; //tiempo en que se monto el disco
    fechActual(part_time);

    if (mountNodo.mnt_particion.part_fit == 'B' || mountNodo.mnt_particion.part_fit == 'F' || mountNodo.mnt_particion.part_fit == 'W') {//es primaria

        part_type = mountNodo.mnt_particion.part_type; //e o p
        part_inicio = mountNodo.mnt_particion.part_start; //inicio
        part_tamano = mountNodo.mnt_particion.part_size; //tamaño d ela particion
        part_colocacion = mountNodo.mnt_particion.part_fit; //w,b,
        part_espacioEbr = 0; //si es extendida
        part_status = 1; //activa o no
        strcpy(part_name, mountNodo.mnt_particion.part_name); //nombre de la partición.

    } else {//del ebr

        part_type = 'L'; //e o p
        part_inicio = mountNodo.mnt_ebr.part_start; //inicio
        part_tamano = mountNodo.mnt_ebr.part_size; //tamaño d ela particion
        part_colocacion = mountNodo.mnt_ebr.part_fit; //w,b, hay que buscar la particion extendida
        part_espacioEbr = sizeof (ebr); //si es extendida
        part_status = 1; //activa o no
        strcpy(part_name, mountNodo.mnt_particion.part_name); //nombre de la partición.
    }
    particionMontada retorno;

    retorno.part_type = part_type; //e o p
    retorno.part_inicio = part_inicio; //inicio
    retorno.part_tamano = part_tamano; //tamaño d ela particion
    retorno.part_colocacion = part_colocacion; //w,b,
    retorno.part_espacioEbr = part_espacioEbr; //si es extendida
    retorno.part_status = part_status; //activa o no

    strcpy(retorno.part_name, part_name); //nombre de la partición.
    strcpy(retorno.ruta, mountNodo.mnt_ruta); //ruta del disco
    strcpy(retorno.id, id); //id del disco
    strcpy(retorno.part_time, part_time); //tiempo en que se monto el disco

    return retorno;
}

void fechActual(times fecha) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    strcpy(fecha,buf);
}