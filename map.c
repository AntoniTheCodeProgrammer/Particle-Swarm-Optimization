#include "map.h"

void gen_map(char *file_name){
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Błąd tworzenia pliku mapy!\n");
        exit(1);
    }

    int width = 10;
    int height = 10;

    // size input
    printf("Podaj szerokosc: ");
    scanf("%d", &width);
    printf("Podaj wysokość: ");
    scanf("%d", &height);

    fprintf(file, "%d %d\n", width, height);

    // random peak
    int peak_x = rand() % width;
    int peak_y = rand() % height;

    double max_height = 100.0;

    // creating values and adding to map
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double dist = sqrt(pow(x - peak_x, 2) + pow(y - peak_y, 2));
            
            double val = max_height - (dist * 2.0);
            
            double noise = (((double)rand() / RAND_MAX) * 20.0) - 10.0;
            val += noise;

            fprintf(file, "%.2lf ", val);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void create_map(Map *my_map, char *file_name){
    // check to map gen
    if(strcmp(file_name, "gen") == 0){
        file_name = "gen_map.txt";
        gen_map(file_name);
    }

    FILE *fptr = fopen(file_name, "r");
    if (fptr == NULL) {
        printf("Nie udalo sie otworzyc pliku mapy");
        exit(1);
    }

    if (fscanf(fptr, "%d %d", &my_map->width, &my_map->height) != 2) {
        printf("Blad formatu naglowka mapy\n");
        exit(1);
    }

    my_map->values = (double**)malloc(my_map->height * sizeof(double*));
    if (my_map->values == NULL) {
        printf("Blad alokacji pamieci (wiersze)");
        exit(1);
    }

    // insert into Map structure
    for(int y = 0; y < my_map->height; y++){
        my_map->values[y] = (double*)malloc(my_map->width * sizeof(double));
        if (my_map->values[y] == NULL) {
            printf("Blad alokacji pamieci (kolumny)");
            exit(1);
        }

        for(int x = 0; x < my_map->width; x++){
            if (fscanf(fptr, "%lf", &my_map->values[y][x]) != 1) {
                printf("Blad odczytu wartosci w punkcie [%d, %d]\n", x, y);
                exit(1);
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
    // Coordinates of left upper point
    int row = (int)position.y;
    int col = (int)position.x;

    if (row < 0 || row > my_map->height - 1 || col < 0 || col > my_map->width - 1) {
        return -10000.0;
    }

    double value_x_1;
    double value_x_2;
    double value_final;

    // Distance to left side
    double x_to = position.x - col;

    // Secure to no go outside Map
    int safe_next_col = col + 1;
    if (safe_next_col >= my_map->width) safe_next_col = col;

    int safe_next_row = row + 1;
    if (safe_next_row >= my_map->height) safe_next_row = row;

    // Value of upper border
    value_x_1 = my_map->values[row][col] * (1 - x_to) + my_map->values[row][safe_next_col] * x_to;

    // Value of bottom border
    value_x_2 = my_map->values[safe_next_row][col] * (1 - x_to) + my_map->values[safe_next_row][safe_next_col] * x_to;
    
    // Horizontal distance 
    double y_to = position.y - row;

    // Final value
    value_final = value_x_1 * (1 - y_to) + value_x_2 * y_to;
    
    return value_final;
}