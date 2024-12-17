#include <stdio.h>
#include <stdlib.h>
#include "neighbors.h"
#include "relation.h"
#include <limits.h>


//struct to take for any cell all directions of her neighbors
struct neighbors_dir{
  enum dir_t d[MAX_NEIGHBORS+1];
};

//table of all directions neighbors of all cells
struct neighbors_dir tab[WIDTH*HEIGHT]; 


void init_neighbors(unsigned int seed){
  if(seed==1){
     for(int i=0;i<WIDTH*HEIGHT;i++){
   tab[i].d[0]=EAST ;
   tab[i].d[1]=NEAST;
   tab[i].d[2]=NORTH;
   tab[i].d[3]=NWEST;
   tab[i].d[4]=WEST;
   tab[i].d[5]=SWEST;
   tab[i].d[6]=SOUTH ;
   tab[i].d[7]=SEAST ;
   tab[i].d[8]=NO_DIR;
  }
  }
  if(seed==2){
  for(int i=0;i<WIDTH*HEIGHT;i++){
  tab[i].d[0]=NEAST;
   tab[i].d[1]=NWEST;
   tab[i].d[2]=SWEST;
   tab[i].d[3]=SEAST;
   tab[i].d[4]=DOUBLE_WEST;
  tab[i].d[5]=DOUBLE_EAST;
   tab[i].d[6]=NO_DIR;
  }
  }

  else if(seed==3){
    for(int i=0;i<WIDTH*HEIGHT;i++){
   tab[i].d[0]=NEAST;
   tab[i].d[1]=NWEST;
   tab[i].d[2]=SWEST;
   tab[i].d[3]=SEAST;
   tab[i].d[4]=NO_DIR;
  } 
  }

  
}



unsigned int get_neighbor(unsigned int idx, enum dir_t d){
    if((int)idx<0) return UINT_MAX;
  switch(d){
  case 0:
    return UINT_MAX;      //NO_DIR
  case 11:
    return UINT_MAX;    // MAX_DIR

  case 1:
    if((idx+1)%WIDTH==0) return UINT_MAX;  //idx in the last column
    return idx+1;
  case 2:
    if( (idx+1)%WIDTH==0 || idx<WIDTH ) return UINT_MAX; //idx in the last column or in first line
    return idx-WIDTH+1;
  case 3:
    if(idx<WIDTH) return UINT_MAX; //idx in first line
    return idx-WIDTH;
  case 4:
    if(idx%WIDTH==0 || idx<WIDTH ) return UINT_MAX; //idx in the first column or in first line
    return idx-WIDTH-1;
  case -1:
    if(idx%WIDTH==0) return UINT_MAX; // idx in the first column
    return idx-1;
  case -2:
    if(idx%WIDTH==0 || idx>=(HEIGHT-1)*(WIDTH)) return UINT_MAX; //idx in the first column or in the last line
    return idx+WIDTH-1;
  case -3:
    if(idx>=(HEIGHT-1)*WIDTH) return UINT_MAX; //idx in the last line
    return idx+WIDTH;
  case -4:
    if((idx+1)%WIDTH==0 || idx>=(HEIGHT-1)*WIDTH) return UINT_MAX; // idx in the last line or in the last column
    return idx+WIDTH+1;
  case 5: 
    if((idx+1)%WIDTH==0 || (idx+2)%WIDTH==0) return UINT_MAX;
    return idx+2;
  case -5:
    if(idx%WIDTH==0  || (idx-1)%WIDTH==0) return UINT_MAX;
    return idx-2;
  }
  return UINT_MAX;
}


struct neighbors_t get_neighbors(unsigned int idx){
  struct neighbors_t s; 
  int i=0;
    int seed=type_relation(); // the type of relation

    int tmp=0;
  init_neighbors(seed);
  while(i<MAX_NEIGHBORS+1 && tab[idx].d[i]!=NO_DIR){
    if(get_neighbor(idx,tab[idx].d[i])!=UINT_MAX){
      s.n[tmp].d=tab[idx].d[i];
      s.n[tmp].i=get_neighbor(idx,tab[idx].d[i]);
      tmp++;
    }
  i++;
  }

  s.n[tmp].d=NO_DIR;
  s.n[tmp].i=UINT_MAX;
    
   
  return s;

}




unsigned int get_neighbors_seed(){
      return 0;
}


 