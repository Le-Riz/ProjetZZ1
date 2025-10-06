CC=gcc
SRC=Fenetre.c bullet.c
EXE=Prog

CFLAGS=-Wall -Wextra -g
LDFLAGS=-lm -lSDL2 -lSDL2_image

OBJ=$(addprefix build/,$(SRC:.c=.o))
DEP=$(addprefix build/,$(SRC:.c=.d))

all: $(OBJ)
	$(CC) -o $(EXE) $^ $(LDFLAGS)

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf build core *.o

-include $(DEP)
