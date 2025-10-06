CC=gcc
SRC=main.c SDL.c draw.c collision.c texture.c geometrie.c background.c Menu.c game.c
EXE=Limule's Adventure

CFLAGS=-Wall -Wextra -g
LDFLAGS=-lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx

OBJ=$(addprefix build/,$(SRC:.c=.o))
DEP=$(addprefix build/,$(SRC:.c=.d))

all: $(OBJ)
	$(CC) -o "$(EXE)" $^ $(LDFLAGS)

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf build core *.o

-include $(DEP)
