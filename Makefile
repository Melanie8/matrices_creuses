CC=gcc
CFLAGS=-Wall -Werror
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
EXEC=matrixprod

all: $(EXEC)

matrixprod: $(OBJ)
	    @$(CC) -o -o $@ $^

main.o : smatrix.h queue.h

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)