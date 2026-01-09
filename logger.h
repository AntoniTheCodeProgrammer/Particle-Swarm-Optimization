#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#include "struct.h"

void save_to_file(char* file_name, Swarm *my_swarm, int save_nr);

#endif