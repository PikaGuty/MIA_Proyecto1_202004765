//
// Created by javierg on 5/06/22.
//

#ifndef MIA_PROYECTO1_202004765_ESTRUCTURAS_H
#define MIA_PROYECTO1_202004765_ESTRUCTURAS_H

#include <iostream>
#include <string.h>
#include <time.h>

typedef char times[50];

typedef struct{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
} partitiond;

typedef struct{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_dsk_signature;
    partitiond mbr_partition_1;
    partitiond mbr_partition_2;
    partitiond mbr_partition_3;
    partitiond mbr_partition_4;
} mbr;

typedef struct {
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
} ebr;

typedef struct{
    int i_uid;     //UID del propietario
    int i_gid;      //GID al grupo al que pertenece

    int i_size;     //tamaño del archivo en bytes
    times i_atime;  //ultima fecha que se leyó el inodo sin modificarlo
    times i_ctime;  //fecha en la que se creó el inodo
    times i_mtime;  //ultima fecha en la que se modifico el inodo
    int i_block[15];//arreglo de punteros, los prieros 12 son directos, 13 indirecto, 14 indirecto doble, 15 indirecto triple
    char i_type;    //1 = archivo, 0 = carpeta
}inodo;

typedef struct{
    char status;
}bmInodo;

typedef struct{
    char status;
}bmBloque;

typedef struct{
    int s_inodes_count;     //Guarda el número total de inodos
    int s_blocks_count;     //Guarda el número total de bloques
    int s_free_blocks_counts;//Contiene el número de bloques libres
    int s_free_inodes_count;//Contiene el número de inodos libres
    times s_mtime;          //Última fecha en el que el sistema fue montado
    times s_unmtime;        //Última fecha en que el sistema fue desmontado
    int s_mnt_count;        //Indica cuantas veces se ha montado el sistema
    int s_magic;            //Valor que identifica al sistema de archivos, tendrá el valor 0xEF53
    int s_inode_size;       //Tamaño del inodo
    int s_block_size;       //Tamaño del bloque
    int s_first_ino;        //Primer inodo libre
    int s_first_blo;        //Primer bloque libre
    int s_bm_inode_start;   //Guardará el inicio del bitmap de inodos
    int s_bm_block_start;   //Guardará el inicio del bitmap de bloques
    int s_inode_start;      //Guardará el inicio de la tabla de inodos
    int s_block_start;      //Guardará el inico de la tabla de bloques
}superBloque;

typedef struct{
    char b_content[64];
}bloqueArchivo;

typedef struct{
    char b_name[12];
    int b_inodo;
}content;

typedef struct{
    content b_content[4];
}bloqueCarpeta;

typedef struct{
    char journal_tipo;
    char journal_nombre[16];
    char journal_contenido;
    times journal_fecha;
    char Padre[12];
}journalie;

typedef struct{
    int b_pointers[16];
}bloqueApuntadores;

typedef struct {
    char part_type;//e o p
    int part_inicio;//inicio
    int part_tamano;//tamaño d ela particion
    char part_colocacion;//w,b,
    int part_espacioEbr;//si es extendida
    int part_status;//activa o no
    char part_name[16];//nombre de la partición.
    char ruta[512];//ruta del disco
    char id[16];//id del disco
    times part_time; //tiempo en que se monto el disco

}particionMontada;

#endif //MIA_PROYECTO1_202004765_ESTRUCTURAS_H
