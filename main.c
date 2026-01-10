#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "struct.h"
#include "map.h"
#include "pso.h"
#include "logger.h"

void print_usage(const char *program_name) {
    printf("Uzycie: %s <plik_mapy lub gen jesli chcesz generowac> [-p liczba_czastek] [-i liczba_iteracji] [-c plik_konfig] [-n co_ile_zapis]\n", program_name);
    printf("Przyklad: %s terrain.txt -p 50 -i 200 -c config.txt -n 2\n", program_name);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char *plik_mapy = NULL;
    int liczba_czastek = 30;
    int liczba_iteracji = 100;
    char *plik_konfiguracyjny = "config_file.txt";
    int zapis_postepow = 0;
    
    int c;
    while ((c = getopt (argc, argv, "p:i:c:n:")) != -1) {
        switch (c) {
            case 'p':
                printf("Option p has option %s\n", optarg);
                liczba_czastek = atoi(optarg);
                break;
            case 'i':
                printf("Option i has option %s\n", optarg);
                liczba_iteracji = atoi(optarg);
                break;
            case 'c':
                printf("Option c has option %s\n", optarg);
                plik_konfiguracyjny = optarg;
                break;
            case 'n':
                printf("Option n has option %s\n", optarg);
                zapis_postepow = atoi(optarg);
                break;
        }
    } 

    printf("%i %i\n", optind, argc);
    if (optind < argc) {
        plik_mapy = argv[optind];
    } else {
        printf("Błąd: Nie podano pliku mapy!\n");
        return 1;
    }   

    if (plik_mapy == NULL) {
        fprintf(stderr, "Blad: Nie podano pliku mapy!\n");
        print_usage(argv[0]);
        return 1;
    }

    return 0;
    // GENEROWANIE MAPY w pliku map.c
    Map my_map;

    create_map(&my_map, plik_mapy);
    // print_map(&my_map);

    // GENEROWANIE DRONOW w pliku struct.c 
    Swarm my_swarm;

    gen_swarm(&my_swarm, &my_map, liczba_czastek, plik_konfiguracyjny);
    // save_to_file function

    // TWORZENIE PLIKU .csv
    char *file_name = "results.csv";

    FILE *file = fopen(file_name, "w");
    if (file != NULL) {
        fprintf(file, "Iteration;Id;X;Y;Value\n");
        fclose(file);
    } else {
        printf("Błąd: Nie można utworzyć pliku wyników.\n");
    }

    // SYMULACJA
    for(int i = 0; i < liczba_iteracji; i++){
        pso(&my_swarm, &my_map, i);

        if(zapis_postepow != 0 && i % zapis_postepow == 0){
            int save_nr = i / zapis_postepow;
            save_to_file(file_name, &my_swarm, save_nr);            
        }
    }

    printf("\n");
    printf("WYNIK\n");
    printf("Szukany obiekt znajduje się na pozycji x = %lf, y = %lf z sygnałem %lf.\n", my_swarm.g_Best_position.x, my_swarm.g_Best_position.y, my_swarm.g_Best_value);
    free_map(&my_map);
    free_swarm(&my_swarm);
};
