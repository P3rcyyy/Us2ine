CC = gcc
CFLAGS = -Wall -Wextra

all: Us2ine affichage

Us2ine: main.c
	$(CC) $(CFLAGS) -o Us2ine main.c -pthread -lrt

affichage:
	$(CC) $(CFLAGS) -o affichage affichage.c -lrt

clean:
	rm -rf affichage
	rm -rf Us2ine