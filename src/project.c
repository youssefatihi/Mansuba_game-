#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "world.h"
#include "neighbors.h"
#include "ensemble.h"
#include "relation.h"
#include "chemin.h"
#include "display.h"
#include "mouvements.h"
#include <limits.h>
#include<getopt.h>
#include<unistd.h>
#include<time.h>

#define PRISONNIERE -1


struct cell_preson{
int type;
enum sort_t sort;
enum sort_t first_sort_died;
};


struct player{
  enum color_t color;   // the color of the player
  int dir;   //the direction of playing (1 from right to left) (2 from left to right )
  struct set begining_places;   
  struct set current_places;
  struct cell_preson t[HEIGHT*WIDTH]; 
};




//to determine if there are cells with 0 neighbors and give them no_cell in sort
void initilize_plateau(struct world_t *world,int seed){
  if(seed!=1){
  struct set set=set_empty(); // the cell with the index 0 is the first availeble cell
  set_add(&set,0);
  for(int i=0;i<WIDTH*HEIGHT;i++){ // the loop to determine all cells that they have at least one neighbor
    for(int j=0;j<set.size;j++){
      struct neighbors_t s=get_neighbors(set.t[j]);
      for(int k=0;s.n[k].d!=NO_DIR;k++)
        if(!set_find(set,s.n[k].i)) set_add(&set,s.n[k].i);
    } 

  }

    for(int j=0;j<WIDTH*HEIGHT;j++){ //give no_cell to the cells with 0 neighbors
      if(!set_find(set,j)) world_set_sort(world,j,NO_CELL);
    } 
  }
}





 





//initialize the player and the begining places in the world
void initialize_player(int dir,struct player* player, struct world_t* world, int tour_number, int elephant_number, int horse_number){
  player->dir=dir;
  player->begining_places.size = 0;
  player->current_places.size = 0;
  int tmp;
  if(dir==1){ // dir=1  the player play from the left to right 
    tmp=0;
    player->color=BLACK; 
  }
  else{ // the player play from the right to left   
    tmp=WIDTH-1;
    player->color=WHITE;
  }
  for(int i=0;i<WIDTH*HEIGHT;i++){
  player->t[i].type=1;
   player->t[i].sort=NO_SORT;
   player->t[i].first_sort_died=NO_SORT;
  }
  int k=0;
  while(k<=WIDTH*(HEIGHT-1)+tmp){
    if(world_get_sort(world,k+tmp)!=NO_CELL){ //Color the cells of the first or the last column
    world_set_sort(world,k+tmp,PAWN); 
    player->t[k+tmp].sort=PAWN;
    world_set(world,k+tmp,player->color);
    set_add(&player->begining_places,k+tmp);
    set_add(&player->current_places,k+tmp);
    }
    k=k+WIDTH;   
  }
 
  //for adding TOUR cells
  int i=0;
  int tmp_rand;
  while(i<tour_number){ // to add exactly tour_number cells with sort=TOUR
    tmp_rand=rand()%HEIGHT;
    if(tmp_rand<player->current_places.size && world_get_sort(world,player->current_places.t[tmp_rand])==PAWN ){ // while we didn't find a available place
        world_set_sort(world,player->current_places.t[tmp_rand],TOUR);
         player->t[player->current_places.t[tmp_rand]].sort=TOUR;
        i++;
    }
  }
  
  //for adding ELEPHANT cells
  int j=0;
  tmp_rand=0;
   while(j<elephant_number){
    tmp_rand=rand()%HEIGHT;
    if(tmp_rand<player->current_places.size && world_get_sort(world,player->current_places.t[tmp_rand])==PAWN ){
        world_set_sort(world,player->current_places.t[tmp_rand],ELEPHANT);
        player->t[player->current_places.t[tmp_rand]].sort=ELEPHANT;
           j++;
    }    
  }
//for adding HORSE cells
  int z=0;
  tmp_rand=0;
   while(z<horse_number){
    tmp_rand=rand()%HEIGHT;
    if(tmp_rand<player->current_places.size && world_get_sort(world,player->current_places.t[tmp_rand])==PAWN){
        world_set_sort(world,player->current_places.t[tmp_rand],HORSE);
        player->t[player->current_places.t[tmp_rand]].sort=HORSE;
        z++;
    } 
  } 
  
} 


// returns 1 if the game is over else 0
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



// for printing the number of prisoner cells
void print_table(struct player player){
  int l=0;
  for(int i=0;i<WIDTH*HEIGHT;i++){
    if(player.t[i].type==PRISONNIERE) l++;
  }
  printf("PRESONIERES=%d \n",l);
}


// play function with the shortest path
void play(struct player *player1,struct world_t *world,struct player *other){
  if(player1->current_places.size!=0){  
  int b=rand()%player1->current_places.size; // to choose randomply one index from the current_places
    int a=player1->current_places.t[b]; 
    int tmp=0;
    int move=0; // the move
    int loop=0; // to avoid infinite loop
    struct set set=set_empty(); // set for the possible moves
    
    while(tmp==0){
      //struct set take the possible moves of the index a  
      switch(world_get_sort(world,a)){
        case HORSE:
            set=horse_moves(world,a);
            break;
        case TOUR:
            set=tour_moves(world,a);
            break;
        case ELEPHANT:
            set=elephant_moves(world,a);
            break;
        default:
            set=possible_moves(world,a,other->begining_places);            
      }

      if(set.size!=0 ){ // if possible moves not empty
         move=best_move(world,set,other->begining_places);
        if(move==-1) move=set.t[rand()%set.size]; //move = -1 so all the begining places of the ohter player are busy
      }
      //for the escape and the case is available
      if(player1->t[a].type==PRISONNIERE && world_get_sort(world,a)==NO_SORT){
        if(rand()%2==0){
        //  printf("----------------------------------------------------------------------------------------------------\n");    
          world_set_sort(world,a,player1->t[a].sort);
          world_set(world,a,player1->color);
          player1->t[a].type=1;
        }
         tmp=3;
        }
       //for the escape and the case is not available we search of another cell
        else if(player1->t[a].type==PRISONNIERE && world_get_sort(world,a)!=NO_SORT){
          b=(b+1)%player1->current_places.size; // to choose randomply one index from the current_places
          a=player1->current_places.t[b];  
          loop++;
        }
        //for not take cells in the begining places of any player
        else if( set_find(other->begining_places,a) || world_get_sort(world,a)==NO_SORT || set_find(player1->begining_places,move) ||((set_find(other->begining_places,move) ) && world_get_sort(world,move)!=NO_SORT)){
          b=(b+1)%player1->current_places.size; // to choose randomply one index from the current_places
          a=player1->current_places.t[b];  
          loop++;
        }
        
        else {
         
          tmp=2;
        }
        //to avoid infinite loop 
        if(loop>=player1->current_places.size+10){
          tmp=3;
        }
      
    }
    
    
    if(tmp==2 && set.size!=0){
      //To capture the opponent
       if(other->t[move].first_sort_died==NO_SORT){ 
       if(world_get_sort(world,move)!=NO_SORT ){
              other->t[move].type=PRISONNIERE; 
          //    printf("__________________________\n");      
         }
        
        if(player1->t[move].type==PRISONNIERE ){
          player1->t[move].first_sort_died=player1->t[move].sort;
        }
         player1->t[move].sort=player1->t[a].sort;
        
        if(player1->t[a].first_sort_died!=NO_SORT)  {
          player1->t[a].sort=player1->t[a].first_sort_died;
          player1->t[a].first_sort_died=NO_SORT;
           player1->t[a].type=PRISONNIERE;
        }
         else {
          player1->t[a].sort=NO_SORT;
         player1->t[a].type=1;
         }
         player1->t[move].type=1;
         player1->current_places.t[b]=move;
         
         world_set(world,move,player1->color);
         
         world_set_sort(world,move,world_get_sort(world,a));
         world_set_sort(world,a,NO_SORT);
         world_set(world,a,NO_COLOR);
         }
       }   
    }
  
  
 // print_table(*player1);
}


int main(int argc,char* argv[]){
  int seed=160; //default value
  char* type="s"; // default value simple victory
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


  
  printf("%d %d\n",WIDTH,HEIGHT);
  printf("#\n"); 
  struct world_t* world=world_init(); //initialize the world
  initilize_plateau(world,type_relation());
  tab_distance(world);
  struct player player1;
  struct player player2;
  int random_player=rand()%2+1; //choosing the player who starts randomly
  int other_player=(random_player%2)+1; // random player and other player must be one of them 1 and the other 2
  int elephant_number=0; // the number of ELEPHANT cells for each player
  int horse_number=0; // the number of HORSE cells for each player
  int tour_number=0;  // the number of TOUR cells for each player 
  if(type_relation()==2 || type_relation()==3) horse_number=tour_number=0;

  
  initialize_player(other_player,&player2,world,tour_number,elephant_number,horse_number);
  initialize_player(random_player,&player1,world,tour_number,elephant_number,horse_number);
  
  
  if(type_relation()==2) display_sexagon(world);
  else{
    display(world);       // for terminal display
    //sdl_display(world);   // sdl diplay
  }
  printf("\n");
  int i;
  for(i=0;i<iterations;i++){ 
    printf("player %d\n",player1.color);
    play(&player1,world,&player2);
  if(type_relation()==2) display_sexagon(world);
  else{
    display(world);  
    printf("\n");     // for terminal display
    //sdl_display(world);   // sdl diplay
  }
  printf("player %d\n",player2.color);
  play(&player2,world,&player1);
  if(type_relation()==2) display_sexagon(world);
  else{
    display(world);  
    printf("\n");     // for terminal display
    //sdl_display(world);   // sdl diplay
  }
  

  if(win_game(player1,player2,type)==1){
      printf("player %d gagne\n",player1.color);
      i=iterations+1;
    } 

   else if(win_game(player2,player1,type)==1){
      printf( "player %d gagne\n",player2.color);
      i=iterations+1;
    }
    printf("\n");
  }
  if(i==iterations) printf("Match nul\n"); 

  return 0;

}

   
