#ifndef __ENSEMBLE_H__
#define __ENSEMBLE_H__

#include "geometry.h"

struct set {
  int t[WIDTH*HEIGHT];
  int size;
};

// return a struct empty set
struct set set_empty();
// return 1 if set is empty,  0 otherwise
int is_empty(struct set);




// add the elemet x to set 
void set_add(struct set* set,int x);
 


// return 1 if x exist in set, 0 otherwise 
int set_find(struct set set, int x);


//translates elements in [begin..end] to [begin-1..end-1] 
void shift_left(struct set *set, int  begin);
//remove the element c from the set se if exists
void set_remove(struct set *se, int c);

//add all elements of set2 to set1
struct set add_sets(struct set set1, struct set set2);


//remove all elements of tmp from set
void remove_sets(struct set *set,struct set *tmp);


#endif
