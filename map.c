#include "map.h"

void gen_map(char *file_name){
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Błąd tworzenia pliku mapy!\n");
        exit(1);
    }

    int width = 10;
    int height = 10;

    printf("Podaj szerokosc: ");
    scanf("%d", &width);
    printf("Podaj wysokość: ");
    scanf("%d", &height);

    fprintf(file, "%d %d\n", width, height);

    int peak_x = rand() % width;
    int peak_y = rand() % height;

    double max_height = 100.0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double dist = sqrt(pow(x - peak_x, 2) + pow(y - peak_y, 2));
            
            double val = max_height - (dist * 2.0);
            
            double noise = ((double)rand() / RAND_MAX) * 10.0;
            val += noise;

            fprintf(file, "%.2lf ", val);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void create_map(Map *my_map, char *file_name){
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
        fprintf(stderr, "Blad formatu naglowka mapy\n");
        exit(1);
    }

    my_map->values = (double**)malloc(my_map->height * sizeof(double*));
    if (my_map->values == NULL) {
        printf("Blad alokacji pamieci (wiersze)");
        exit(1);
    }

    for(int y = 0; y < my_map->height; y++){
        my_map->values[y] = (double*)malloc(my_map->width * sizeof(double));
        if (my_map->values[y] == NULL) {
            printf("Blad alokacji pamieci (kolumny)");
            exit(1);
        }

        for(int x = 0; x < my_map->width; x++){
            if (fscanf(fptr, "%lf", &my_map->values[y][x]) != 1) {
                fprintf(stderr, "Blad odczytu wartosci w punkcie [%d, %d]\n", x, y);
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
    // Koordynaty wieżchołka lewego gornego 
    int row = (int)position.y;
    int col = (int)position.x;

    if (row < 0 || row > my_map->height - 1 || col < 0 || col > my_map->width - 1) {
        return -10000.0;
    }

    double value_x_1;
    double value_x_2;
    double value_final;

    // Odległość od lewego wierzchołka w poziomie
    double x_to = position.x - col;

    // Zabezpieczenia wyscia poza indexy
    int safe_next_col = col + 1;
    if (safe_next_col >= my_map->width) safe_next_col = col;

    int safe_next_row = row + 1;
    if (safe_next_row >= my_map->height) safe_next_row = row;

    // Wartość na gornej krawedzi
    value_x_1 = my_map->values[row][col] * (1 - x_to) + my_map->values[row][safe_next_col] * x_to;

    // Wartość na dolnej krawedzi
    value_x_2 = my_map->values[safe_next_row][col] * (1 - x_to) + my_map->values[safe_next_row][safe_next_col] * x_to;
    
    // Odległość od lewego wierzchołka w pionie
    double y_to = position.y - row;

    value_final = value_x_1 * (1 - y_to) + value_x_2 * y_to;
    
    
    return value_final;
}