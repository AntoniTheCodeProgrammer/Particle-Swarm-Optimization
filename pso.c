#include "pso.h"

void gen_swarm(Swarm *my_swarm, Map *my_map, int particles_count, char *config_file){
    // CONFIG FILE read
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
    
    // Swarm init
    my_swarm->particle_count = particles_count;
    my_swarm->particles = (Particle*)malloc(particles_count * sizeof(Particle));
    
    // Best point init
    Coordinates best_position;
    double best_position_value = -9999999;

    for(int i = 0; i < particles_count; i++){
        my_swarm->particles[i].index = i;

        // Posting random position
        double x = (double)(rand() % my_map->width);
        double y = (double)(rand() % my_map->height);

        Coordinates position;
        position.x = x;
        position.y = y;

        my_swarm->particles[i].position = position;

        // Value ge
        double value = get_map_value(my_map, position); 
        my_swarm->particles[i].value = value;

        // Changing velocity to zero
        Coordinates velocity;
        velocity.x = 0;
        velocity.y = 0;
        my_swarm->particles[i].velocity = velocity;

        // Best point set
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
    for(int i = 0; i < my_swarm->particle_count; i++){

        // Random r in range of [0,1]
        double r1 = (double)rand() / RAND_MAX; 
        double r2 = (double)rand() / RAND_MAX;

        // Calculating velocity
        double x = (
            my_swarm->w * my_swarm->particles[i].velocity.x + 
            my_swarm->c_1 * r1 * (my_swarm->particles[i].pBest_position.x - my_swarm->particles[i].position.x) + 
            my_swarm->c_2 * r2 * (my_swarm->g_Best_position.x - my_swarm->particles[i].position.x)
        );

        double y = (
            my_swarm->w * my_swarm->particles[i].velocity.y + 
            my_swarm->c_1 * r1 * (my_swarm->particles[i].pBest_position.y - my_swarm->particles[i].position.y) + 
            my_swarm->c_2 * r2 * (my_swarm->g_Best_position.y - my_swarm->particles[i].position.y)
        );

        // speed limit
        if (x > 1){
            x = 1;
        }
        if (x < -1){
            x = -1;
        }
        if (y > 1){
            y = 1;
        }
        if (y < -1){
            y = -1;
        }

        // rng bonus from Antos Casino Engineer
        if(my_swarm->bonus_mode == 1){
            double x_rng = ((((double)rand() / RAND_MAX) * 2) - 1) * interation / my_swarm->iteration_MAX;
            double y_rng = ((((double)rand() / RAND_MAX) * 2) - 1) * interation / my_swarm->iteration_MAX;
            printf("%i %i %lf %lf\n", interation, my_swarm->iteration_MAX, x_rng, y_rng);
            x += x_rng;
            y += y_rng;
        }
        

        // Move and Velocity
        my_swarm->particles[i].velocity.x = x;
        my_swarm->particles[i].velocity.y = y;

        my_swarm->particles[i].position.x += x;
        my_swarm->particles[i].position.y += y;

        // Border limit
        if (my_swarm->particles[i].position.x < 0) {
            my_swarm->particles[i].position.x = 0;
        } 
        else if (my_swarm->particles[i].position.x >= my_map->width - 1) {
            my_swarm->particles[i].position.x = my_map->width - 1;
        }


        if (my_swarm->particles[i].position.y < 0) {
            my_swarm->particles[i].position.y = 0;
        } 
        else if (my_swarm->particles[i].position.y >= my_map->height - 1) {
            my_swarm->particles[i].position.y = my_map->height - 1;
        }

        // Get value
        my_swarm->particles[i].value = get_map_value(my_map, my_swarm->particles[i].position);

        // Best point update
        if(my_swarm->particles[i].pBest_value < my_swarm->particles[i].value){
            my_swarm->particles[i].pBest_value = my_swarm->particles[i].value;
            my_swarm->particles[i].pBest_position = my_swarm->particles[i].position;

            if(my_swarm->g_Best_value < my_swarm->particles[i].value){
                my_swarm->g_Best_value = my_swarm->particles[i].value;
                my_swarm->g_Best_position = my_swarm->particles[i].position;
            }
        }       
    }
}

void free_swarm(Swarm *my_swarm){
    free(my_swarm->particles);
}
