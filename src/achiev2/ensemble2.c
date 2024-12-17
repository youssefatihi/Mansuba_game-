#include <stdio.h>
#include "ensemble.h"


struct set set_empty(){
  struct set set;
  set.size=0;
  return set;
}

int is_empty(struct set set){
  return set.size==0;
}

void set_add(struct set* set,int x){
  set->t[set->size]=x;
  set->size++;
}


int set_find(struct set set,int x){
  if(set.size==0) return 0;
  for(int i=0;i<set.size;i++){
    if(set.t[i]==x) return 1;
  }
  return 0;
}





void shift_left(struct set *set, int  begin){
  int a=begin;
  while(a!=set->size){
    set->t[a-1]=set->t[a];
    a++;    
  }
  set->t[a-1]=set->t[a];
}



void set_remove(struct set *se, int c){
  int a=0;
  for(int i=0;i<se->size;i++){
    if(se->t[i]==c){
      shift_left(se,i+1);
      a=1;
    }
  }
  if(a==1) se->size--;

}


struct set add_sets(struct set set1, struct set set2){
  if(set2.size==0) return set1;
  if(set1.size==0) return set2;
  for(int i=0;i<set2.size;i++){
    set_add(&set1,set2.t[i]);
  }
  return set1;
}

void remove_sets(struct set *set,struct set *tmp){
    if(!is_empty(*tmp)){
        for(int i=0;i<tmp->size;i++) set_remove(set,tmp->t[i]);
    }
    tmp->size=0;
}
