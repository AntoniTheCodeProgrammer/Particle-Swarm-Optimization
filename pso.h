#ifndef PSO_H
#define PSO_H

#include <stdio.h>
#include <stdlib.h>

#include "struct.h"
#include "map.h"

void gen_swarm(Swarm *my_swarm, Map *my_map, int particles_count, char *config_file);
void pso(Swarm *my_swarm, Map *my_map, int interation);
void free_swarm(Swarm *my_swarm);

#endif