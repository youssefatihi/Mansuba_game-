#ifndef __CHEMIN_H__
#define __CHEMIN_H__
#include "geometry.h"
#include "world.h"

struct place_distance{
  int place;
  int distance;
};

struct set T[WIDTH*HEIGHT];

int distance(int a, int b);

void tab_distance(struct world_t *world);

struct place_distance minimal_distance(struct set set,int x);

int best_move(struct world_t *world,struct set set1, struct set set2);

#endif
