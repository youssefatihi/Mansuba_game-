#include <stdio.h>
#include <stdlib.h>
#include "mouvements.h"
#include <limits.h>



// returns the neighbors with the sort=NO_SORT
struct set possible_neighbors(struct world_t* w, unsigned int idx){
  struct set set=set_empty();
  struct neighbors_t N=get_neighbors(idx);
  for(int k=0;(int)N.n[k].i!=(int)UINT_MAX;k++){ //the loop to traverse all neighbors
    unsigned int tmp=N.n[k].i;
    if(world_get_sort(w,tmp)==NO_SORT){   //to take only the free neighbors
      set_add(&set,tmp);
    }
  }
  return set;
}


// returns the possible positions with a jump of a pawn over another other color different to the color of the PAWN 
struct set  possible_jumps(struct world_t* w, unsigned int idx,enum color_t color){
  struct set set=set_empty();
  struct neighbors_t N=get_neighbors(idx);
  unsigned int tmp;
  for(int k=0;(int)N.n[k].i!=(int)UINT_MAX;k++){ // the loop to traverse the all neighbors
    tmp=N.n[k].i;
    if(world_get_sort(w,tmp)!=NO_SORT && world_get(w,tmp)!=color){ // to take juste the pawns with a color different compared to color
      enum dir_t dir=N.n[k].d;
      unsigned int neighbor_after=get_neighbor(tmp,dir);
      if(neighbor_after!=UINT_MAX &&  world_get(w,neighbor_after)!=color) // the neighbor exist and with a color different to color 
      set_add(&set,neighbor_after); 
    }
  }
  return set;
} 

//the possible moves of a pawn cell idx in the world w
struct set possible_moves(struct world_t *w,unsigned int idx,struct set set_begining){
  struct set set=possible_neighbors(w,idx); // to take the possible neighbors
  struct set set2=possible_jumps(w,idx,world_get(w,idx));
  for(int i=0;i<4;i++){ // the loop for doing a lot of jumps here the most we can do is 4 jumps
    if(!is_empty(set2)){
      for(int j=0;j<set2.size;j++){ // To verify if there are other possible jumps from the new cells
        if(!set_find(set_begining,set2.t[j])){ //if the cell is in the begining places of the other player, we don't search for other jump from the cell 
	          set2=add_sets(set2,possible_jumps(w,set2.t[j],world_get(w,idx))); // to add the possible jumps of the set2.t[i] to the others jumps
       }
      }
    }
  }
 for(int i=0;i<set2.size;i++) set_remove(&set2,idx); // for delete the possibility of returning to initail position
 
  return add_sets(set2,set); // add possible jumps and possible neighbors 
 
}


//returns the possibles moves if  sort=HORSE ( the horse play like the letter L)
struct set horse_moves(struct world_t *w,unsigned int idx){
    struct set tmp=set_empty();
    struct set set=set_empty();
    enum dir_t a=1;
    enum dir_t b=3;
   
    set_add(&tmp,get_neighbor(get_neighbor(get_neighbor(idx,a),a),b));   //for the case with two jumps to the right and one to the north
    set_add(&tmp,get_neighbor(get_neighbor(get_neighbor(idx,a),a),-b));  // for the case with two jumps to the right and one to the south
    set_add(&tmp,get_neighbor(get_neighbor(get_neighbor(idx,-a),-a),b)); // for the case with two jumps to the left and one to the north
    set_add(&tmp,get_neighbor(get_neighbor(get_neighbor(idx,-a),-a),-b)); //for the case with two jumps to the left and one to the south

    set_add(&tmp,get_neighbor(get_neighbor(get_neighbor(idx,b),b),a)); //for the case with two jumps to the north and one to right
    set_add(&tmp,get_neighbor(get_neighbor(get_neighbor(idx,b),b),-a)); // for the case with two jumps to the north and one to left
    set_add(&tmp,get_neighbor(get_neighbor(get_neighbor(idx,-b),-b),a)); // for the case with two jumps to the south and one to right
    set_add(&tmp,get_neighbor(get_neighbor(get_neighbor(idx,-b),-b),-a)); //for the case with two jumps to the south and one to left
    for(int i=0; i<tmp.size;i++){
        if(tmp.t[i]!=(int)UINT_MAX &&  world_get(w,tmp.t[i])!=world_get(w,idx)) set_add(&set,tmp.t[i]); // to take just the available cells 
    }
    
    return set; 
}

//returns the possible moves if sort=TOUR 
struct set tour_moves(struct world_t *w, int idx){
    struct set set=set_empty();
    enum dir_t dir;
    int i=-3;
     int tmp;
    while(i<=3){
        dir=i;
        tmp=idx;
        while(get_neighbor(tmp,dir)!=UINT_MAX && ( world_get_sort(w,get_neighbor(tmp,dir))==NO_CELL || world_get(w,get_neighbor(tmp,dir))!=world_get(w,idx))){ 
            if(world_get_sort(w,get_neighbor(tmp,dir))!=NO_CELL) set_add(&set, get_neighbor(tmp,dir));
            tmp=get_neighbor(tmp,dir);
        }
      i=i+2;   // to take just{-3,-1,1,3}
        
    }
    return set;
}

//returns the possible moves if sort=ELEPHANT
struct set elephant_moves(struct world_t *w,int idx){
enum dir_t dir1;
enum dir_t dir2;
struct set set=set_empty();
int i=-3;
while(i<=3){ // the loop to take the cells with to jumps in same direction 
    dir1=i;
    if(get_neighbor(get_neighbor(idx,dir1),dir1)!=UINT_MAX && world_get(w,get_neighbor(get_neighbor(idx,dir1),dir1))!=world_get(w,idx)) 
        set_add(&set,get_neighbor(get_neighbor(idx,dir1),dir1));
    
    if(i>=0) { // to take cells with two deffrent directions for exemple {SOUTH+EST, SOUTH+WEST}
         dir2=-(i+1);
         dir1=i+1;
        if(get_neighbor(idx,dir2)!=UINT_MAX && world_get(w,get_neighbor(idx,dir2))!=world_get(w,idx))
             set_add(&set,get_neighbor(idx,dir2));
        else if(get_neighbor(idx,dir1)!=UINT_MAX && world_get(w,get_neighbor(idx,dir1))!=world_get(w,idx))
         set_add(&set,get_neighbor(idx,dir1));
    }
    i=i+2;
    }   
    return set;

}
