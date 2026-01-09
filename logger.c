#include "logger.h"

void save_to_file(char *file_name, Swarm *my_swarm, int save_nr){
    FILE *file = fopen(file_name, "a");

    if (file == NULL) {
        printf("Błąd: Nie można otworzyć pliku %s do zapisu!\n", file_name);
        return; 
    }

    for(int i = 0; i < my_swarm->particle_count; i++){
        fprintf(file, "%i;%i;%lf;%lf;%lf\n", 
            save_nr,
            my_swarm->particles[i].index, 
            my_swarm->particles[i].position.x, 
            my_swarm->particles[i].position.y, 
            my_swarm->particles[i].value
        );
    }
    // printf("saved\n");

    fclose(file);

}

