CC = gcc
CFLAGS = -Wall -Wextra

all: Us2ine affichage

Us2ine: main.c
	$(CC) $(CFLAGS) -o Us2ine main.c -pthread -lrt -lm

affichage:
	$(CC) -o affichage affichage.c -lrt 

clean:
	rm -rf affichage
	rm -rf Us2ine