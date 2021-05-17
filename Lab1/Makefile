SRC = pthread1.c
OBJ = pthread1.o
# DEPS = 
CFLAGS = -lpthread
CC = gcc

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)

pthread: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
