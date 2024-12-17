#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "world.h"
#include "neighbors.h"
#include "ensemble.h"
#include "geometry.h"
#include <limits.h>
#include<getopt.h>
#include<unistd.h>
#include<time.h>

// Constants for the terminal display
#define ANSI_COLOR_RED     "\e[41m"
#define ANSI_COLOR_BLUE    "\e[44m"
#define ANSI_COLOR_BLACK  "\e[40m"
#define ANSI_COLOR_RESET   "\e[0m"



struct player{
  enum color_t color;   // the color of the player
  int dir;   //the direction of playing (1 from right to left) (2 from left to right )
  struct set begining_places;   
  struct set current_places;
};
/*
// struct palce respresent a cell in the world and c is the color of the cell with s represent the sort
struct place{
  enum color_t c;
  enum sort_t s;
};

// a world with WIDTH*HEIGHT cells
struct world_t{
  struct place t[WIDTH*HEIGHT];
};
*/
// return the neighbors with the sort=NO_SORT
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


// return the possible positions with a jump over another other pawn 
struct set  possible_jumps(struct world_t *w, unsigned int idx){
  struct set set=set_empty();
  struct neighbors_t N=get_neighbors(idx);
  unsigned int tmp;

  for(int k=0;(int)N.n[k].i!=(int)UINT_MAX;k++){ // the loop to traverse the all neighbors
    tmp=N.n[k].i;
   if(world_get_sort(w,tmp)!=NO_SORT && world_get(w,tmp)!=world_get(w,idx)){ // to take juste the pawns with a color different compared to idx
      enum dir_t dir=N.n[k].d;
      unsigned int neighbor_after=get_neighbor(tmp,dir);

     if(neighbor_after!=UINT_MAX &&  world_get_sort(w,neighbor_after)==NO_SORT )set_add(&set,neighbor_after); // the neighbor exist and available
  
    }
  }
  return set;
    

} 
 

//the possible moves of the cell idx in the world w
struct set possible_moves(struct world_t *w,unsigned int idx,struct player other){
  struct set set=possible_neighbors(w,idx);
  struct set set2=possible_jumps(w,idx);
  for(int i=0;i<4;i++){ // the loop for doing a lot of jumps 
     if(!is_empty(set2)){
      for(int j=0;j<set2.size;j++){ // To verify if there are other possible jumps from the new cells
        if(!set_find(other.begining_places,set2.t[j])){
            world_set(w,set2.t[j],world_get(w,idx));
            world_set_sort(w,set2.t[j],world_get_sort(w,idx));
	          set2=add_sets(set2,possible_jumps(w,set2.t[j]));
            world_set(w,set2.t[j],NO_COLOR); 
            world_set_sort(w,set2.t[j],NO_SORT);
      }
      }
    }
  }
  for(int i=0;i<set2.size;i++) set_remove(&set2,idx); // for delete the possibility of returning to initail position
  
  return add_sets(set2,set); // add possible jumps and possible neighbors
  
}




//initialize the player and the begining places in the world
void initialize_player(int dir,struct player* player, struct world_t* world){
  player->dir=dir;
  player->begining_places.size = 0;
  player->current_places.size = 0;
  int tmp;
  if(dir==1){ // dir=1  the player play from the left to right 
    tmp=0;
    player->color=BLACK; 
  }
  else{ // the player play from right to left   
    tmp=WIDTH-1;
    player->color=WHITE;
  }
  int k=0;
  while(k<=WIDTH*(HEIGHT-1)+tmp){ //Color the cells of the first or the last column
    world_set(world,k+tmp,player->color);
    world_set_sort(world,k+tmp,PAWN);
    set_add(&player->begining_places,k+tmp);
    set_add(&player->current_places,k+tmp);

    k=k+WIDTH;   
  }

} 
// return 1 if the game is over else 0
int win_game(struct player current_player,struct player last_player,char* type){
  if(!strcmp(type,"s")){ // s for a simple win
    for(int i=0;i<current_player.current_places.size;i++){
      if(set_find(last_player.begining_places,current_player.current_places.t[i])) 
	return 1;
    }
    return 0;
  }

  else{ // Complex Victory
    for(int i=0;i<current_player.current_places.size;i++){
      if(!set_find(last_player.begining_places,current_player.current_places.t[i]))
	return 0;
    }
    return 1;
  } 


} 


//display fonction (terminal diplay)
void display(struct world_t *world){
  for(int i=0;i<WORLD_SIZE;i++){
    if(i%WIDTH==0 && i!=0) printf("\n");
    if(world_get(world,i)==BLACK)
      printf(ANSI_COLOR_BLACK "  " ANSI_COLOR_RESET);
    if(world_get(world,i)==WHITE)
      printf(ANSI_COLOR_RED "  " ANSI_COLOR_RESET);
    if(world_get(world,i)==NO_COLOR)
      printf(ANSI_COLOR_BLUE "  " ANSI_COLOR_RESET);

  }
  printf("\n");

}

//sdl display 
void sdl_display(struct world_t *world){
  int n=0;   // color black in sdl
  int b=16777215; // wolor white 
  int g=255;   // color red 
   
  for(int i=0;i<WORLD_SIZE;i++){
    if(i%WIDTH==0 && i!=0) printf("\n");
    if(world_get(world,i)==BLACK)
      printf("%d ",n );
    if(world_get(world,i)==WHITE)
      printf("%d ",b);
    if(world_get(world,i)==NO_COLOR)
      printf("%d ",g );

  }
  printf("\n");

}



// play function with rand moves
void play(struct player *player1,struct world_t *world,struct player other){
  if(player1->current_places.size!=0){  
    int b=rand()%player1->current_places.size; // to choose randomply an index from the current_places

    int a=player1->current_places.t[b]; 
    while(set_find(other.begining_places,a)){
      b=rand()%player1->current_places.size;
      a=player1->current_places.t[b]; 
    }
    struct set set=possible_moves(world,a,other); //determine the possible moves
    int move=1;
    if(set.size!=0){ // if possible moves not empty
      move=rand()%set.size;
      player1->current_places.t[b]=set.t[move];
      world_set(world,a,NO_COLOR);
       world_set_sort(world,a,NO_SORT);
      
      world_set(world,set.t[move],player1->color);
      world_set_sort(world,set.t[move],PAWN);
    } 
    
  }
}




int main(int argc,char* argv[]){
  int seed=160; //default values
  char* type="s";
  int iterations=2*WORLD_SIZE;

  char stropt[]="mts";
  int c;
  while((c=getopt(argc,argv,stropt))!=EOF){
    switch(c){
    case 'm':
      iterations=atoi(argv[optind]);
      break;
    case 's':
      seed=atoi(argv[optind]);
      break;
    case 't':
      type=argv[optind];
      break;
    }}
     
  srand(seed);    
  struct world_t* world=world_init(); //initialize the world
  struct player player1;
  struct player player2;
  int random_player=rand()%2+1; //choosing the player who starts randomly
  int other_player=(random_player)%2+1;

  initialize_player(random_player,&player1,world); 
  initialize_player(other_player,&player2,world);
  printf("%d %d\n",WIDTH,HEIGHT);
  printf("#\n");
   display(world);       // for terminal display
   //sdl_display(world);   // sdl diplay
  printf("\n");
  int i;
  for(i=0;i<iterations;i++){ 
    play(&player1,world,player2);
    play(&player2,world,player1);
     display(world);              //terminal display
     // sdl_display(world);              //sdl diplay
    if(win_game(player1,player2,type)){
      printf("player1 gagne\n");
      i=iterations+1;
    }
    else if(win_game(player2,player1,type)){
      printf( "player2 gagne\n");
    }
    printf("\n");

  }
  if(i==iterations) printf("Match nul\n"); 

  return 0;

}

   
 
