#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "map.h"
#include "pso.h"

void print_usage(const char *program_name) {
    printf("Uzycie: %s <plik_mapy> [-p liczba_czastek] [-i liczba_iteracji] [-c plik_konfig] [-n co_ile_zapis]\n", program_name);
    printf("Przyklad: %s terrain.txt -p 50 -i 200 -c config.txt -n 2\n", program_name);
}

int main(int argc, char *argv[]) {
    char *plik_mapy = "test_map.txt";
    int liczba_czastek = 30;
    int liczba_iteracji = 100;
    char *plik_konfiguracyjny = "config_file.txt";
    int zapis_postepow = 0;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            if (i + 1 < argc) {
                liczba_czastek = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Blad: Oczekiwano wartosci po fladze -p\n");
                return 1;
            }
        } 
        else if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 < argc) {
                liczba_iteracji = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Blad: Oczekiwano wartosci po fladze -i\n");
                return 1;
            }
        } 
        else if (strcmp(argv[i], "-c") == 0) {
            if (i + 1 < argc) {
                plik_konfiguracyjny = argv[++i];
            } else {
                fprintf(stderr, "Blad: Oczekiwano sciezki po fladze -c\n");
                return 1;
            }
        } 
        else if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 < argc) {
                zapis_postepow = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Blad: Oczekiwano wartosci po fladze -n\n");
                return 1;
            }
        }
        else if (plik_mapy == NULL && argv[i][0] != '-') {
            plik_mapy = argv[i];
        } 
        else {
            fprintf(stderr, "Ostrzezenie: Nieznany argument lub zduplikowany plik mapy: %s\n", argv[i]);
        }
    }

    if (plik_mapy == NULL) {
        fprintf(stderr, "Blad: Nie podano pliku mapy!\n");
        print_usage(argv[0]);
        return 1;
    }

    // GENEROWANIE MAPY w pliku map.c
    Map my_map;

    create_map(&my_map, plik_mapy);
    print_map(&my_map);

    // GENEROWANIE DRONOW w pliku struct.c 
    Swarm my_swarm;

    gen_swarm(&my_swarm, &my_map, liczba_czastek, plik_konfiguracyjny);
    // save_to_file function

    // SYMULACJA
    for(int i = 0; i < liczba_iteracji; i++){
        // ruch dronow plik pso.c

        if(zapis_postepow != 0 && i % zapis_postepow == 0){
            // zapis plik logger.c
        }
        
    }

    free_map(&my_map);
};
