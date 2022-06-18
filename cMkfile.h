//
// Created by javierg on 12/06/22.
//

#ifndef MIA_PROYECTO1_202004765_CMKFILE_H
#define MIA_PROYECTO1_202004765_CMKFILE_H

#include "estructuras.h"
#include "cMount.h"
#include "cMkfs.h"
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fstream>


void cMkfile(char path[512], char id[16], bool p, int size, char cont[512]);


#endif //MIA_PROYECTO1_202004765_CMKFILE_H
