//
// Created by javierg on 12/06/22.
//

#include "cMkdir.h"

particionMontada devolverParticionMontada(char id[16]);
void fechActual(char fecha[128]);
void crearRoot(char id[16]);
void verSB(superBloque sb);

void cMkdir(char path[512], char id[16], bool p){
    particionMontada m= devolverParticionMontada(id);


    crearRoot(id);

    superBloque sb = sb_retornar(id);
    mnt_nodo mountNodo = retornarNodoMount(id);
    int n = sb.s_inodes_count; //Total de inodos
    inodo ino[n];
    inodos_leer(sb.s_inode_start,n,mountNodo.mnt_ruta,ino);

    cout<<"UID del usuario: "<<ino[0].i_uid<<endl;
    cout<<"GID del grupo: "<<ino[0].i_gid<<endl;
    cout<<"Tamaño del archivo: "<<ino[0].i_size<<endl;
    cout<<"Última fecha en que se leyó el inodo sin modificarlo: "<<ino[0].i_atime<<endl;
    cout<<"Fecha en la que se creó el inodo: "<<ino[0].i_ctime<<endl;
    cout<<"Última fecha en la que de modificación del inodo: "<<ino[0].i_mtime<<endl;
    for (int j = 0; j < 15; ++j) {
        if(j<12){
            cout<<"AD"<<j<<": "<<ino[0].i_block[j]<<endl;
            if(ino[0].i_block[j]!=-1){
                cout<<"ESCRIBIR 2 "<<ino[0].i_block[j]<<endl;
                cout<<"********************************** CARPETA **********************************"<<endl;
                bloqueCarpeta carpeta=blocksC_leer(ino[0].i_block[j],n,mountNodo.mnt_ruta,carpeta);
                cout<<"Nombre 1: "<<carpeta.b_content[0].b_name<<endl;
                cout<<"Inodo 1: "<<carpeta.b_content[0].b_inodo<<endl;
                cout<<"Nombre 2: "<<carpeta.b_content[1].b_name<<endl;
                cout<<"Inodo 2: "<<carpeta.b_content[1].b_inodo<<endl;
                cout<<"Nombre 3: "<<carpeta.b_content[2].b_name<<endl;
                cout<<"Inodo 3: "<<carpeta.b_content[2].b_inodo<<endl;
                cout<<"Nombre 4: "<<carpeta.b_content[3].b_name<<endl;
                cout<<"Inodo 4: "<<carpeta.b_content[3].b_inodo<<endl;
                cout<<"****************************************************************************"<<endl;
            }
        }else{
            cout<<"AI"<<j<<": "<<ino[0].i_block[j]<<endl;
            if(ino[0].i_block[j]!=-1){
                cout<<"********************************** CARPETA **********************************"<<endl;
                bloqueCarpeta carpeta;
                blocksC_leer(ino[0].i_block[j],n,mountNodo.mnt_ruta,carpeta);
                cout<<"Nombre 1: "<<carpeta.b_content[0].b_name<<endl;
                cout<<"Inodo 1: "<<carpeta.b_content[0].b_inodo<<endl;
                cout<<"Nombre 2: "<<carpeta.b_content[1].b_name<<endl;
                cout<<"Inodo 2: "<<carpeta.b_content[1].b_inodo<<endl;
                cout<<"Nombre 3: "<<carpeta.b_content[2].b_name<<endl;
                cout<<"Inodo 3: "<<carpeta.b_content[2].b_inodo<<endl;
                cout<<"Nombre 4: "<<carpeta.b_content[3].b_name<<endl;
                cout<<"Inodo 4: "<<carpeta.b_content[3].b_inodo<<endl;
                cout<<"****************************************************************************"<<endl;
            }
        }

    }
    cout<<"Tipo: "<<ino[0].i_type<<endl;



    //
    /*
    cout<<"------------------ ParticionMontada: Id="<<m.id<<" ------------------"<<endl;
    cout<<"\t\tRuta: "<<m.ruta<<endl;
    cout<<"\t\tNombre: "<<m.part_name<<endl;
    cout<<"\t\tTiempoDeMontaje: "<<m.part_time<<endl;
    cout<<"\t\tTipo = "<< m.part_type<<"\tInicio= "<<m.part_inicio<<"\tTamño= "<<m.part_tamano<<endl;
    cout<<"\t\tColcn= "<<m.part_colocacion<<"\tEspEbr= "<<m.part_espacioEbr<<"\tStatus= "<<m.part_status<<endl;
    */
}

void crearRoot(char id[16]) {
    //primero tengo que obtener el super bloque
    superBloque sb = sb_retornar(id);
    mnt_nodo mountNodo = retornarNodoMount(id); //la particion que tiene los datos
    int n = sb.s_inodes_count; //Total de inodos

    inodo ino[n];


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
    times fech;
    fechActual(fech);
    strcpy(Inodos[0].i_mtime,fech);
    Inodos[0].i_type=0;
    Inodos[0].i_block[0]= sb.s_first_blo;//primer bloque libre
    Inodos[0].i_type='0';
    inodos_escribir(sb.s_inode_start, n, mountNodo.mnt_ruta, Inodos);//escribiendo lo inodos

    cout<<"ESCRIBIR 1 "<<sb.s_block_start<<endl;
    //indos
    bloqueCarpeta blocks;
    strcpy(blocks.b_content[0].b_name,".");
    blocks.b_content[0].b_inodo = sb.s_inode_start;
    strcpy(blocks.b_content[1].b_name,"..");
    blocks.b_content[1].b_inodo = sb.s_inode_start;
    strcpy(blocks.b_content[2].b_name,"");
    blocks.b_content[2].b_inodo = -1;
    strcpy(blocks.b_content[3].b_name,"");
    blocks.b_content[3].b_inodo = -1;
    blocksC_escribir(sb.s_block_start, n, mountNodo.mnt_ruta, blocks);


    //hay que modificar el super bloque
    sb.s_free_blocks_counts--;
    sb.s_free_inodes_count--;
    sb.s_first_ino=sb.s_first_ino+sizeof(inodo)+1;
    sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta)+1;

    int inicio=0;
    if (mountNodo.mnt_particion.part_fit == 'B' || mountNodo.mnt_particion.part_fit == 'F' || mountNodo.mnt_particion.part_fit == 'W') {//es primaria
        inicio = mountNodo.mnt_particion.part_start;
    } else {//del ebr
        inicio = mountNodo.mnt_ebr.part_start;
    }

    sb_escribir(mountNodo.mnt_ruta,inicio,sb);



    /*cout<<"******************* FINAL *******************"<<endl;
    verSB(sb);
    cout<<endl;
    cout<<endl;

    cout<<"******************* INODO *******************"<<endl;
    for (int i = 0; i < 5; ++i) {
        cout<<"UID del usuario: "<<ino[i].i_uid<<endl;
        cout<<"GID del grupo: "<<ino[i].i_gid<<endl;
        cout<<"Tamaño del archivo: "<<ino[i].i_size<<endl;
        cout<<"Última fecha en que se leyó el inodo sin modificarlo: "<<ino[i].i_atime<<endl;
        cout<<"Fecha en la que se creó el inodo: "<<ino[i].i_ctime<<endl;
        cout<<"Última fecha en la que de modificación del inodo: "<<ino[i].i_mtime<<endl;
        for (int j = 0; j < 15; ++j) {
            if(j<12){
                cout<<"AD"<<j<<": "<<ino[i].i_block[j]<<endl;
            }else{
                cout<<"AI"<<j<<": "<<ino[i].i_block[j]<<endl;
            }

        }
        cout<<"Archivo o carpeta: "<<ino[i].i_type<<endl;
    }*/


}

void verSB(superBloque sb){
    cout<<"No total inodos: "<<sb.s_inodes_count<<endl;
    cout<<"No total bloques: "<<sb.s_blocks_count<<endl;
    cout<<"No inodos libres: "<<sb.s_free_inodes_count<<endl;
    cout<<"No bloques libres: "<<sb.s_free_blocks_counts<<endl;
    cout<<"Ultima fecha montada: "<<sb.s_mtime<<endl;
    cout<<"Ultima fecha desmontado: "<<sb.s_unmtime<<endl;
    cout<<"No veces montado: "<<sb.s_mnt_count<<endl;
    cout<<"No identificador SA: "<<sb.s_magic<<endl;
    cout<<"Tamaño del inodo: "<<sb.s_inode_size<<endl;
    cout<<"Tamaño del bloque: "<<sb.s_block_size<<endl;
    cout<<"Primer inodo libre: "<<sb.s_first_ino<<endl;
    cout<<"Primer bloque libre: "<<sb.s_first_blo<<endl;
    cout<<"Inicio del bitmap inodos: "<<sb.s_bm_inode_start<<endl;
    cout<<"Inicio del bitmap bloques: "<<sb.s_bm_block_start<<endl;
    cout<<"Inicio tabla de inodos: "<<sb.s_inode_start<<endl;
    cout<<"Inicio tabla de bloques: "<<sb.s_block_start<<endl;
    cout<<"Padre: "<<sb.s_bjpurfree<<endl;
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

