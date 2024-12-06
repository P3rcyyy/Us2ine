CC = gcc
CFLAGS = -Wall -Wextra

all: Us2ine

Us2ine: main.c
	$(CC) $(CFLAGS) -o Us2ine main.c -lpthread