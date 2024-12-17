#ifndef __MOUVEMENTS_H__
#define __MOUVEMENTS_H__
#include "world.h"
#include "ensemble.h"
#include "neighbors.h"


struct set possible_neighbors(struct world_t* w, unsigned int idx);


struct set  possible_jumps(struct world_t* w, unsigned int idx,enum color_t color);

struct set possible_moves(struct world_t *w,unsigned int idx,struct set set_begining);


struct set horse_moves(struct world_t *w,unsigned int idx);

struct set tour_moves(struct world_t *w, int idx);

struct set elephant_moves(struct world_t *w,int idx);
#endif
