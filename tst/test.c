#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ensemble.h"
#include "world.h"
#include "neighbors.h"
#include "chemin.h"
#include <limits.h>


static void test_dir_to_string(void){
enum dir_t a=0;
enum dir_t b=1;
enum dir_t c=2;
enum dir_t d=3;
enum dir_t e=4;
assert(strcmp(dir_to_string(a),"NO_DIR")==0);
assert(strcmp(dir_to_string(b),"EAST")==0);
assert(strcmp(dir_to_string(c),"NEAST")==0);
assert(strcmp(dir_to_string(d),"NORTH")==0);
assert(strcmp(dir_to_string(e),"NWEST")==0);
printf("dir_to_string passed\n");
}

static void test_place_to_string(void){
enum color_t c=0;
enum sort_t s=0;
enum color_t d=1;
assert(strcmp(place_to_string(c,s),"Empty_place")==0);
assert(strcmp(place_to_string(d,s),"Black_place")==0);
printf("place_to_string passed\n");

}

static void test_get_neighbor(void){
  unsigned int idx=0;
  enum dir_t dir1=1;
  enum dir_t dir2=2;
  enum dir_t dir3=3;
  enum dir_t dir4=4;

  assert(get_neighbor(idx,dir1)==1);
  assert(get_neighbor(idx,dir2)==UINT_MAX);
  assert(get_neighbor(idx,dir3)==UINT_MAX);
  assert(get_neighbor(idx,dir4)==UINT_MAX);
  printf("get_neighbor passed\n");

}

static void test_get_neighbors(void){
  int idx=0;
  struct neighbors_t str=get_neighbors(idx);
  struct set set=set_empty();
  for(int i=0;str.n[i].i!=UINT_MAX;i++) set_add(&set,str.n[i].i);
 // assert(set_find(set,1)==1);
  assert(set_find(set,WIDTH+3)==0);
  assert(set_find(set,WIDTH+1)==1);
  //assert(str.n[3].i==UINT_MAX && str.n[3].d==NO_DIR);
  printf("get_neighbors passed\n"); 
}


static void test_world_init(void){
  struct world_t* world=world_init();
  assert(world_get(world,0)==NO_COLOR );
  assert(world_get_sort(world,0)==NO_SORT);
  assert(world_get(world,WIDTH)==NO_COLOR );
  assert( world_get_sort(world,WIDTH)==NO_SORT);
  assert(world_get(world,WORLD_SIZE-1)==NO_COLOR);
  assert( world_get_sort(world,WORLD_SIZE-1)==NO_SORT);
  printf("world init passed\n");
}

static void test_world_get(void){
  struct world_t *world=world_init();
  world_set(world,WIDTH,BLACK);
  world_set_sort(world,WIDTH,PAWN);
  world_set(world,WORLD_SIZE-1,WHITE);
  world_set_sort(world,WORLD_SIZE-1,PAWN);
  assert(world_get(world,0)==NO_COLOR);
  assert(world_get(world,WIDTH)==BLACK);
  assert(world_get(world,WORLD_SIZE-1)==WHITE);
  printf("world_get passed\n");
}
static void test_world_get_sort(void){
  struct world_t *world=world_init();
  world_set(world,WIDTH,BLACK);
  world_set_sort(world,WIDTH,PAWN);
  world_set(world,WORLD_SIZE-1,WHITE);
  world_set_sort(world,WORLD_SIZE-1,PAWN);
  
  assert(world_get_sort(world,0)==NO_SORT);
  assert(world_get_sort(world,WIDTH)==PAWN);
  assert(world_get_sort(world,WORLD_SIZE-1)==PAWN);
  printf("world_get_sort passed\n");
 // display(*world);
} 





void afficher_set(struct set set){
  for(int i=0;i<set.size;i++)
  printf("%d ", set.t[i]);
  printf("\n");
  }




static void test_stategie(void){
  
  struct world_t* world=world_init();
  tab_distance(world);
  struct set set1={{0,11,48,22},4};
  //struct set set=set_empty();
  struct set set2={{4*WIDTH-1,2*WIDTH-1,6*WIDTH-1},3};
  //printf("best 11 move %d %d\n",minimal_distance(set1,set2.t[1]).place,minimal_distance(set1,set2.t[1]).distance);
  printf("best move %d\n",best_move(world,set1,set2) );
 //printf("best move %d  %d\n",minimal_distance(set2,25).place, minimal_distance(set2,25).distance);
 
}





int main(){
  
  printf("WI=%d Hei=%d\n",WIDTH,HEIGHT);
  test_get_neighbor();
  test_get_neighbors();
  test_dir_to_string();
  test_place_to_string();
  test_get_neighbor();
  test_world_init();
  test_world_get();
  test_world_get_sort();  
  
 test_stategie();
  return 0;
}


