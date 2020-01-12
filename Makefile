BASE_FLAGS=-std=gnu99 -Wall -Wextra -pedantic -lm -g
KANYE_FLAGS=-pthread `sdl2-config --cflags --libs`
KANYE_LINKS=vectors.o matrices.o transforms.o shaders.o pipeline.o

.DEFAULT_GOAL := all

vectors:
	gcc -c vectors.c $(BASE_FLAGS)

matrices:
	gcc -c matrices.c $(BASE_FLAGS)

transforms:
	gcc -c transforms.c $(BASE_FLAGS)

shaders:
	gcc -c shaders.c $(BASE_FLAGS)

pipeline:
	gcc -c pipeline.c $(BASE_FLAGS)

kanye:
	gcc -o kanye3 kanye3.c $(KANYE_LINKS) $(BASE_FLAGS) $(KANYE_FLAGS)

all: vectors matrices transforms shaders pipeline kanye