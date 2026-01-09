#include "map.h"

void gen_map(char *file_name){

}

void create_map(Map *my_map, char *file_name){
    if(strcmp(file_name, "gen") == 0){
        file_name = "gen_map.txt";
        gen_map(file_name);
    }
    
    FILE *fptr = fopen(file_name, "r");
    if (fptr == NULL) {
        printf("Nie udalo sie otworzyc pliku mapy");
        return;
    }

    if (fscanf(fptr, "%d %d", &my_map->width, &my_map->height) != 2) {
        fprintf(stderr, "Blad formatu naglowka mapy\n");
        return;
    }

    my_map->values = (double**)malloc(my_map->height * sizeof(double*));
    if (my_map->values == NULL) {
        printf("Blad alokacji pamieci (wiersze)");
        return;
    }

    for(int y = 0; y < my_map->height; y++){
        my_map->values[y] = (double*)malloc(my_map->width * sizeof(double));
        if (my_map->values[y] == NULL) {
            printf("Blad alokacji pamieci (kolumny)");
            return;
        }

        for(int x = 0; x < my_map->width; x++){
            if (fscanf(fptr, "%lf", &my_map->values[y][x]) != 1) {
                fprintf(stderr, "Blad odczytu wartosci w punkcie [%d, %d]\n", x, y);
                return;
            }
        }
    }

    fclose(fptr);
}

void print_map(Map *my_map){
    for(int y = 0; y < my_map->height; y++){
        for(int x = 0; x < my_map->width; x++){
            printf("%.2f ", my_map->values[y][x]);
        }
        printf("\n");
    }
}

void free_map(Map *my_map){
    for(int y = 0; y < my_map->height; y++){
        free(my_map->values[y]);
    }

    free(my_map->values);
}

double get_map_value(Map *my_map, Coordinates position){
    // mozna zrobic bardziej zaawansowany system usredniajacy wartosci
    int row = (int)position.y;
    int col = (int)position.x;

    if (row < 0 || row >= my_map->height || col < 0 || col >= my_map->width) {
        return -10000.0;
    }

    return my_map->values[row][col];
}