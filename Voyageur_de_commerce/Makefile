CC=gcc
SRC=main.c graph.c SDL.c draw.c collision.c SDLGraph.c texture.c
EXE=Prog

CFLAGS=-Wall -Wextra -g
LDFLAGS=-lm -lSDL2 -lSDL2_image -lSDL2_ttf

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
