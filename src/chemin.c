#include <stdio.h>
#include <stdlib.h>
#include "ensemble.h"
#include "chemin.h"
#include "neighbors.h"
#include "world.h"


int distance(int a, int b){
  struct set set=set_empty(); // the cell with the index 0 is the first availeble cell
    set_add(&set,a);
    struct set set_tmp=set;
    int tmp=0;
    while(!set_find(set,b) && tmp<WIDTH*HEIGHT){
    for(int j=0;j<set_tmp.size;j++){
      struct neighbors_t s=get_neighbors(set.t[j]);
      for(int k=0;s.n[k].d!=NO_DIR;k++)
        if(!set_find(set,s.n[k].i)) set_add(&set,s.n[k].i);
    }
    set_tmp=set;
    tmp++; 
    }
    if(tmp==WIDTH*HEIGHT) return -1;
    return tmp;
  }

void tab_distance(struct world_t *world){
  for(int i=0;i<WORLD_SIZE;i++){
    T[i].size=WORLD_SIZE;
    for(int j=i;j<WORLD_SIZE;j++){
        if(world_get_sort(world,i)==NO_CELL || world_get_sort(world,j)==NO_CELL){
         T[i].t[j]=-1;
         T[j].t[i]=-1;
        }
        else{
      T[i].t[j]=distance(i,j);
      T[j].t[i]=distance(j,i);
        }
    }
  }
}

struct place_distance minimal_distance(struct set set,int x){
  struct place_distance s={-1,-1};
  if(is_empty(set)) return s;
  int tmp=set.t[0];
  for(int i=1; i<set.size; i++){
    if(T[set.t[i]].t[x]<T[tmp].t[x]) {
      tmp=set.t[i];
  }
  }
  s.place=x;
  s.distance=T[tmp].t[x];
  return s;
}

int best_move(struct world_t *world,struct set set1, struct set set2){
struct set set_tmp=set_empty();
for(int i=0;i<set2.size;i++){
  if(world_get_sort(world,set2.t[i])==NO_SORT) set_add(&set_tmp,set2.t[i]);
}
set2=set_tmp;
//printf("il y a %d  cases libre pour l'adversaire \n",set2.size);
//printf("%d\n",set_tmp.size);
if(is_empty(set2)) return -1;
struct place_distance tmp=minimal_distance(set2,set1.t[0]);
for(int i=1;i<set1.size;i++){
  if(minimal_distance(set2,set1.t[i]).distance<tmp.distance){
    tmp.distance=minimal_distance(set2,set1.t[i]).distance;
    tmp.place=set1.t[i];
  }
}
return tmp.place;
}