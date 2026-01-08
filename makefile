CC = gcc
CFLAGS = -Wall -g

pso: main.o map.o pso.o
	$(CC) $(CFLAGS) -o pso main.o map.o pso.o

main.o: main.c map.h struct.h pso.h
	$(CC) $(CFLAGS) -c main.c

map.o: map.c struct.h
	$(CC) $(CFLAGS) -c map.c

pso.o: pso.c map.h struct.h
	$(CC) $(CFLAGS) -c pso.c

clean:
	rm -f *.o pso
