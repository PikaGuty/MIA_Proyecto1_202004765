//
// Created by javierg on 12/06/22.
//

#include "cMkdir.h"


void fechActual(char fecha[128]);


void cMkdir(char path[512], char id[16], bool p){
    //particionMontada m= devolverParticionMontada(id);
    superBloque sb = sb_retornar(id); //Obteniendo super bloque
    mnt_nodo mountNodo = retornarNodoMount(id); //Obteniendo nodo de la partición montada

    if(strcmp(mountNodo.mnt_ruta,"") == 0||sb.s_inodes_count==-1){
        return;
    }

    string auxf = path;
    size_t pos = 0;
    string res = "";
    auxf.erase(0, pos+1);
    int n = sb.s_inodes_count; //Total de inodos
    inodo ino[n];
    inodos_leer(sb.s_inode_start,n,mountNodo.mnt_ruta,ino); //Obteniendo la lista de inodos en el sistema

    inodo inodoActual=ino[0];
    datosBusquedaCarpeta busqueda, creada;
    busqueda.encontrada= false;
    char carpe[32]="";
    while ((pos = auxf.find("/")) != string::npos) {
        //cout<<"Carpeta "<<auxf.substr(0, pos)<<endl; //Carpetas que hay que verificar que existan o crearlas

        strcpy(carpe,auxf.substr(0, pos).c_str());

        busqueda=buscarCarpeta(carpe, inodoActual, p, n,mountNodo.mnt_ruta);

        auxf.erase(0, pos + 1); //eliminamos del buffer la carpeta que estabamos buscando
        if(busqueda.encontrada==true){
            inodoActual = busqueda.inod; //Cambiamos de inodo actual para seguir buscando o crear la carpeta que interesa
        }else{
            if(p==true){
                creada = crearCarpeta(carpe, inodoActual, p, n,mountNodo.mnt_ruta, id);
                if (creada.encontrada==true){
                    inodoActual = creada.inod;
                }
            }else{
                cout<<"Error: No existe la carpeta "<<carpe<<endl;
                break;
            }
        }
    }

    if(busqueda.encontrada==true||(creada.encontrada==true&&p==true)) {
        //cout<<"Carpeta "<<auxf.substr(0, pos)<<endl; //Carpeta a crear
        strcpy(carpe, auxf.substr(0, pos).c_str());
        crearCarpeta(carpe, inodoActual, p, n, mountNodo.mnt_ruta, id);
    }


    //cout<<endl;
    //cout<<endl;

    //inodos_leer(sb.s_inode_start,n,mountNodo.mnt_ruta,ino);

    /*cout<<"UID del usuario: "<<ino[0].i_uid<<endl;
    cout<<"GID del grupo: "<<ino[0].i_gid<<endl;
    cout<<"Tamaño del archivo: "<<ino[0].i_size<<endl;
    cout<<"Última fecha en que se leyó el inodo sin modificarlo: "<<ino[0].i_atime<<endl;
    cout<<"Fecha en la que se creó el inodo: "<<ino[0].i_ctime<<endl;
    cout<<"Última fecha en la que de modificación del inodo: "<<ino[0].i_mtime<<endl;
    for (int j = 0; j < 15; ++j) {
        if(j<12){
            cout<<"AD"<<j<<": "<<ino[0].i_block[j]<<endl;
            if(ino[0].i_block[j]!=-1){

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
    cout<<"Tipo: "<<ino[0].i_type<<endl;*/

}

datosBusquedaCarpeta buscarCarpeta(char carpeta[12], inodo inodoActual, bool p, int n, char ruta[512]){
    datosBusquedaCarpeta res;
    res.encontrada=false;
    for (int i = 0; i < sizeof(inodoActual.i_block); i++) { //Buscando primer nodo
        if(inodoActual.i_block[i]!=-1){ //buscando el primer ocupado
            bloqueCarpeta bloque;
            bloque = blocksC_leer(inodoActual.i_block[i], n, ruta, bloque);
            for (int j = 0; j < 4; ++j) {
                if (strcmp(bloque.b_content[j].b_name,carpeta) == 0){ //Verificando si existe en este bloque
                    res.encontrada=true;
                    res.inod = inodos_leer1(bloque.b_content[j].b_inodo, n, ruta, res.inod);
                    break;
                }
            }
        }
        if(res.encontrada==true){
            break;
        }
    }
    return res;
}

datosBusquedaCarpeta crearCarpeta(char carpeta[12], inodo inodoActual, bool p, int n, char ruta[512], char id[16]){

    datosBusquedaCarpeta res;
    res.encontrada=false;
    bool lleno = true; //Bandera para identificar si esta llenos los bloques que ya existen
    for (int i = 0; i < sizeof(inodoActual.i_block); i++) { //Buscando inodoAcrual
        superBloque sb = sb_retornar(id); //Obteniendo super bloque
        mnt_nodo mountNodo = retornarNodoMount(id); //Obteniendo nodo de la partición montada

        if(inodoActual.i_block[i]!=-1){ //buscando el primer ocupado
            bloqueCarpeta bloque;
            bloque = blocksC_leer(inodoActual.i_block[i], n, ruta, bloque);
            for (int j = 0; j < 4; ++j) { //Recorriendo los cuatro espacios del bloque carpeta
                if (bloque.b_content[j].b_inodo==-1){ //Buscando el primer espacio libre
                    strcpy(bloque.b_content[j].b_name,carpeta);

                    //ACTUALIZANDO BLOQUE EXISTENTE
                    strcpy(bloque.b_content[j].b_name,carpeta);
                    bloque.b_content[j].b_inodo=sb.s_first_ino;

                    blocksC_escribir(inodoActual.i_block[i],n,ruta,bloque);
                    //*****************************

                    //CREANDO NUEVO INODO
                    inodo ino;
                    times fech;
                    fechActual(fech);
                    ino.i_uid = 1;
                    ino.i_gid = 1;
                    ino.i_size = 0;
                    strcpy(ino.i_atime,fech);
                    strcpy(ino.i_ctime,fech);
                    strcpy(ino.i_mtime,fech);

                    for (int j = 0; j < 15; ++j) {
                        ino.i_block[j]=-1;
                    }
                    ino.i_block[0]=sb.s_first_blo; //AQUI ME QUEDE
                    ino.i_type='0';
                    inodos_escribir1(sb.s_first_ino, n, mountNodo.mnt_ruta, ino);
                    res.inod=inodos_leer1(sb.s_first_ino, n, mountNodo.mnt_ruta, ino);
                    inodoActual=res.inod;
                    actualizarBMI(sb.s_inode_start, sb.s_first_ino, id);
                    //*****************************

                    //CREANDO PRIMER BLOQUE PARA EL NUEVO bloque
                    bloqueCarpeta blocks;
                    strcpy(blocks.b_content[0].b_name,".");
                    blocks.b_content[0].b_inodo = sb.s_first_ino;
                    strcpy(blocks.b_content[1].b_name,"..");
                    blocks.b_content[1].b_inodo = bloque.b_content[0].b_inodo;
                    strcpy(blocks.b_content[2].b_name,"");
                    blocks.b_content[2].b_inodo = -1;
                    strcpy(blocks.b_content[3].b_name,"");
                    blocks.b_content[3].b_inodo = -1;
                    blocksC_escribir(sb.s_first_blo, n, mountNodo.mnt_ruta, blocks);
                    //*****************************

                    //ACTUALIZANDO SUPERBLOQUE
                    sb.s_free_blocks_counts--;
                    sb.s_free_inodes_count--;
                    sb.s_first_ino=sb.s_first_ino+sizeof(inodo)+1;
                    sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta)+1;

                    int inicio=0;
                    if (mountNodo.mnt_particion.part_fit == 'B' || mountNodo.mnt_particion.part_fit == 'F' || mountNodo.mnt_particion.part_fit == 'W') {//es primaria
                        inicio = mountNodo.mnt_particion.part_start;
                    } else {//del ebr
                        inicio = mountNodo.mnt_ebr.part_start+sizeof (ebr);
                    }
                    sb_escribir(mountNodo.mnt_ruta,inicio,sb);
                    //*************************

                    lleno=false;
                    res.encontrada = true;
                    cout<<"\t................ Creando carpeta "<<carpeta<<" ................"<<endl;

                    break;
                }
            }
        }
        if(lleno==false){ //Para salirse si encontró espacio
            break;
        }
    }
    if (lleno==true){
        //cout<<"estan llenos los otros"<<endl;
        for (int i = 0; i < sizeof(inodoActual.i_block); i++) { //Buscando inodo actual
            bloqueCarpeta bloque;
            bloque = blocksC_leer(inodoActual.i_block[i], n, ruta, bloque);

            superBloque sb = sb_retornar(id); //Obteniendo super bloque
            mnt_nodo mountNodo = retornarNodoMount(id); //Obteniendo nodo de la partición montada
            if(inodoActual.i_block[i]==-1) { //buscando el primer libre
                //cout<<"encontré un nodo libre en "<<i<<endl;

                bloqueCarpeta blq;
                blq = blocksC_leer(inodoActual.i_block[0], n, mountNodo.mnt_ruta, blq);
                //TODO crear bloque crear inodo y primer bloque

                //ACTUALIZANDO CONTENIDO DEL INODO ACTUAL
                inodoActual.i_block[i] = sb.s_first_blo;
                times fecha;
                fechActual(fecha);

                strcpy(inodoActual.i_mtime,fecha);
                inodos_escribir1(blq.b_content[0].b_inodo, n, mountNodo.mnt_ruta, inodoActual);
                //*******************************

                //CREANDO NUEVO BLOQUE
                bloqueCarpeta blocks;

                strcpy(blocks.b_content[0].b_name,carpeta);
                blocks.b_content[0].b_inodo = sb.s_first_ino;
                strcpy(blocks.b_content[1].b_name,"");
                blocks.b_content[1].b_inodo = -1;
                strcpy(blocks.b_content[2].b_name,"");
                blocks.b_content[2].b_inodo = -1;
                strcpy(blocks.b_content[3].b_name,"");
                blocks.b_content[3].b_inodo = -1;
                blocksC_escribir(sb.s_first_blo, n, mountNodo.mnt_ruta, blocks);
                //*****************************

                //ACTUALIZANDO SUPERBLOQUE
                sb.s_free_blocks_counts--;
                sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta)+1;

                //CREANDO NUEVO INODO
                inodo ino;
                times fech;
                fechActual(fech);
                ino.i_uid = 1;
                ino.i_gid = 1;
                ino.i_size = 0;
                strcpy(ino.i_atime,fech);
                strcpy(ino.i_ctime,fech);
                strcpy(ino.i_mtime,fech);

                for (int j = 0; j < 15; ++j) {
                    ino.i_block[j]=-1;
                }
                ino.i_block[0]=sb.s_first_blo; //AQUI ME QUEDE
                ino.i_type='0';
                inodos_escribir1(sb.s_first_ino, n, mountNodo.mnt_ruta, ino);
                res.inod=inodos_leer1(sb.s_first_ino, n, mountNodo.mnt_ruta, ino);
                inodoActual=res.inod;
                actualizarBMI(sb.s_inode_start, sb.s_first_ino, id);
                //*****************************

                //CREANDO PRIMER BLOQUE PARA EL NUEVO INODO
                bloqueCarpeta blocks2;
                strcpy(blocks2.b_content[0].b_name,".");
                blocks2.b_content[0].b_inodo = sb.s_first_ino;
                strcpy(blocks2.b_content[1].b_name,"..");
                blocks2.b_content[1].b_inodo = bloque.b_content[0].b_inodo;
                strcpy(blocks2.b_content[2].b_name,"");
                blocks2.b_content[2].b_inodo = -1;
                strcpy(blocks2.b_content[3].b_name,"");
                blocks2.b_content[3].b_inodo = -1;
                blocksC_escribir(sb.s_first_blo, n, mountNodo.mnt_ruta, blocks2);
                //*****************************

                //ACTUALIZANDO SUPERBLOQUE
                sb.s_free_blocks_counts--;
                sb.s_free_inodes_count--;
                sb.s_first_ino=sb.s_first_ino+sizeof(inodo)+1;
                sb.s_first_blo=sb.s_first_blo+sizeof(bloqueCarpeta)+1;

                int inicio=0;
                if (mountNodo.mnt_particion.part_fit == 'B' || mountNodo.mnt_particion.part_fit == 'F' || mountNodo.mnt_particion.part_fit == 'W') {//es primaria
                    inicio = mountNodo.mnt_particion.part_start;
                } else {//del ebr
                    inicio = mountNodo.mnt_ebr.part_start+sizeof (ebr);
                }
                sb_escribir(mountNodo.mnt_ruta,inicio,sb);
                //*************************

                lleno=false;
                res.encontrada = true;
                cout<<"\t................ Creando carpeta "<<carpeta<<" ................"<<endl;

                break;
            }
        }
    }
    return res;
}

void actualizarBMI(int posIni, int posAct, char id[16]){
    superBloque sb = sb_retornar(id);
    mnt_nodo mountNodo = retornarNodoMount(id); //la particion que tiene los datos

    int posicion = (posAct-posIni)/(sizeof(inodo)+1);

    int n = sb.s_inodes_count; //Total de inodos
    bmInodo bm_Inodos[n];
    bmi_leer(sb.s_bm_inode_start, n, mountNodo.mnt_ruta, bm_Inodos);

    bm_Inodos[posicion].status='1';
    bmi_escribir(sb.s_bm_inode_start,n,mountNodo.mnt_ruta,bm_Inodos);
}

void obtenerListaBMI(char id[16], int direccionesInodos[1024]) {
    superBloque sb = sb_retornar(id);
    int posIni = sb.s_inode_start;
    mnt_nodo mountNodo = retornarNodoMount(id); //la particion que tiene los datos

    int n = sb.s_inodes_count; //Total de inodos
    bmInodo bm_Inodos[n];
    bmi_leer(sb.s_bm_inode_start, n, mountNodo.mnt_ruta, bm_Inodos);

    for (int i = 0; i < 1024; ++i) {
        direccionesInodos[i]=0;
    }

    for (int i = 0; i < n; i++) {
        if(bm_Inodos[i].status=='1'){
            for (int j = 0; j < 1024; j++) {
                if(direccionesInodos[j]==0){
                    direccionesInodos[j] = posIni + i * (sizeof (inodo)+1);
                    //cout<<"Le voy a meter "<<(posIni + i * (sizeof (inodo)+1))<<endl;
                    break;
                }
            }
        }
    }
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
        inicio = mountNodo.mnt_ebr.part_start+sizeof (ebr);
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
    //cout<<"Padre: "<<sb.s_bjpurfree<<endl;
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

