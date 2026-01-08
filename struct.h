#ifndef STRUCT_H
#define STRUCT_H

typedef struct{
    int height;
    int width;
    double **values;
} Map;

typedef struct{
    double x;
    double y;
} Coordinates;

typedef struct{
    int index;
    Coordinates velocity;
    Coordinates position;
    double value;
    Coordinates pBest_position;
    double pBest_value;
} Particle;

typedef struct{
    Particle *particles;
    int particle_count;

    Coordinates g_Best_position;
    double g_Best_value;

    double w;
    double c_1;
    double c_2;
} Swarm;

#endif