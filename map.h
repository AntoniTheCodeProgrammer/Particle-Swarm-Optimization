#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "struct.h"


void create_map(Map *my_map, char *file_name);
void print_map(Map *my_map);
void free_map(Map *my_map);
double get_map_value(Map *my_map, Coordinates position);

#endif