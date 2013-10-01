CC=clang
CFLAGS=-Wall -Werror
EXEC=matrixprod
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

matrixprod: $(OBJ)
	    @$(CC) -o $@ $^

smatrix.o : queue.h
read.o : smatrix.h
main.o : smatrix.h queue.h read.h

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)