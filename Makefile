WIDTH ?= 10
HEIGHT ?= 10
RELATION ?=2
GCC = gcc
MANSUBA_FLAGS = -DWIDTH=$(WIDTH) -DHEIGHT=$(HEIGHT) -DRELATION=$(RELATION)
CFLAGS = -Wall -Wextra -std=c99 -g3 $(MANSUBA_FLAGS)

all:project test

world.o: src/world.c
	gcc -c $(CFLAGS) src/world.c

ensemble.o: src/ensemble.c
	gcc -c $(CFLAGS) src/ensemble.c

geometry.o: src/geometry.c
	gcc -c $(CFLAGS) src/geomerty.c
relation.o: src/relation.c
	gcc -c $(CFLAGS) src/relation.c
chemin.o: src/relation.c
	gcc -c $(CFLAGS) src/chemin.c
mouvements.o: src/mouvements.c
	gcc -c $(CFLAGS) src/mouvements.c
display.o: src/display.c
	gcc -c $(CFLAGS) src/display.c


project: src/project.c src/relation.c src/chemin.o src/mouvements.o src/display.o src/world.o src/geometry.o src/ensemble.o src/neighbors.c
	$(GCC) $(CFLAGS) src/project.c src/relation.c src/display.o src/mouvements.o src/chemin.o src/world.o src/geometry.o src/ensemble.o src/neighbors.c -o project


test: test_set test_1 
test_set: tst/test_set.c src/ensemble.o
	gcc $(CFLAGS) -I src tst/test_set.c src/ensemble.o  -o test_set

test_1: tst/test.c src/ensemble.o src/chemin.o src/relation.c src/neighbors.c src/world.o src/geometry.o 
	$(GCC) $(CFLAGS) -I src tst/test.c src/chemin.o src/relation.c src/ensemble.o  src/neighbors.c src/geometry.o src/world.o -o test_1

clean:
	rm -f project test_1  test_set src/*.o *~




