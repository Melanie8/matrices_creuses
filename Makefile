CC=gcc
CFLAGS=-Wall -Werror
EXEC=matrixprod
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

matrixprod: $(OBJ)
	    @$(CC) -o $@ $^

smatrix.o : queue.h
main.o : smatrix.h queue.h

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)