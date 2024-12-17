#include <stdio.h>
#include <stdlib.h>
#include "neighbors.h"
#include <limits.h>

unsigned int get_neighbor(unsigned int idx, enum dir_t d){
  switch(d){
  case 0:
    return UINT_MAX;      //NO_DIR
  case 9:
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
  }
  return UINT_MAX;
}


struct neighbors_t get_neighbors(unsigned int idx){
  struct neighbors_t s; 
  enum dir_t dir;
  int tmp=0; // tmp represent the number of filled cells.
  for(int j=1;j<=4; j++){   // the loop is from 1 to 4 because the directions are {-4,-3,-2,-1,1,2,3,4}
    unsigned int a=get_neighbor(idx,j);  // for the directions {1,2,3,4}
    unsigned int b=get_neighbor(idx,-j); //for the directions {-1,-2,-3,-4}
    if(a!=UINT_MAX){ // neighbor exist
      s.n[tmp].i=a;
      dir=j;  // dir for change the type from int to enum dir
      s.n[tmp].d=dir;
      tmp++;
    }

    if(b!=UINT_MAX){ // neighbor exist
      s.n[tmp].i=b;
      dir=-j;
      s.n[tmp].d=dir;
      tmp++;
    }
  }
    
  s.n[tmp].i=UINT_MAX;  // neighbors end with {UINT_MAX,NO_DIR}
  s.n[tmp].d=NO_DIR;
  return s;
}
