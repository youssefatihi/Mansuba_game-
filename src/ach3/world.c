#include <stdio.h>
#include <stdlib.h>
#include "world.h"
#include "neighbors.h" 

// represent one cell of world with color c and sort s
struct place{
    enum color_t c;
    enum sort_t s;
};

//the world contains WORLD_SIZE cells
struct world_t{
struct place t[WORLD_SIZE];
};


struct world_t* world_init(){
    
    static struct world_t world;
    for(int i=0;i<WORLD_SIZE;i++){
        world.t[i].c=NO_COLOR;
        world.t[i].s=NO_SORT;
    }
    return &world;
}

enum color_t world_get(const struct world_t* b, unsigned int idx){
    return b->t[idx].c;
}


void world_set(struct world_t* b, unsigned int idx, enum color_t c){
    b->t[idx].c=c;
}

enum sort_t world_get_sort(const struct world_t* b, unsigned int idx){
    return b->t[idx].s;
}


void world_set_sort(struct world_t* b, unsigned int idx, enum sort_t c){
    b->t[idx].s=c;
}