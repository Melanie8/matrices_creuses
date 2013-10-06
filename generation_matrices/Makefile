CC=gcc
CFLAGS=-Werror
EXEC=matrixprod
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

matrixprod: $(OBJ)
	    @$(CC) -o $@ $^

read.o : fmatrix.h
main.o : fmatrix.h read.h

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)