#include "pso.h"

void gen_swarm(Swarm *my_swarm, Map *my_map, int particles_count, char *config_file){
    // PLIK KONFIGURACYJNY
    FILE *fptr = fopen(config_file, "r");
    if (fptr == NULL) {
        printf("Nie udalo sie otworzyc pliku konfiguracyjnego");
        exit(1);
    }

    if (fscanf(fptr, "%lf %lf %lf", &my_swarm->w, &my_swarm->c_1, &my_swarm->c_2) != 3) {
        fprintf(stderr, "Blad formatu pliku konfiguracyjnego (oczekiwano: w c1 c2)\n");
        my_swarm->w = 0.5;
        my_swarm->c_1 = 1.0;
        my_swarm->c_2 = 1.0;
    }

    fclose(fptr);
    
    // INICJACJA ROJU
    my_swarm->particle_count = particles_count;
    my_swarm->particles = (Particle*)malloc(particles_count * sizeof(Particle));
        
    Coordinates best_position;
    double best_position_value = -9999999;

    for(int i = 0; i < particles_count; i++){
        my_swarm->particles[i].index = i;

        double x = (double)(rand() % my_map->width);
        double y = (double)(rand() % my_map->height);

        Coordinates position;
        position.x = x;
        position.y = y;

        my_swarm->particles[i].position = position;

        double value = get_map_value(my_map, position); 
        my_swarm->particles[i].value = value;

        Coordinates velocity;
        velocity.x = 0;
        velocity.y = 0;
        my_swarm->particles[i].velocity = velocity;

        my_swarm->particles[i].pBest_position = my_swarm->particles[i].position;
        my_swarm->particles[i].pBest_value = my_swarm->particles[i].value;
        
        if(value > best_position_value){
            best_position_value = value;
            best_position = position;
        }
    }    
    


    my_swarm->g_Best_position = best_position;
    my_swarm->g_Best_value = best_position_value; 
}

void pso(Swarm *my_swarm, Map *my_map, int interation){
    
}