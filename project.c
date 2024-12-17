#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "world.h"
#include "neighbors.h"
#include "ensemble.h"
#include "geometry.h"
#include "relation.h"
#include <limits.h>
#include<getopt.h>
#include<unistd.h>
#include<time.h>

#define PRESONNIERE -1

// Constants for the terminal display 
//to color the writing
#define ANSI_WHITE     "\e[0;37m"//"\e[47;30m"
#define ANSI_BLUE   "\e[0;34m"// "\e[44m"
#define ANSI_RED    "\e[0;31m"//"\e[41m"
#define ANSI_BLACK   "\e[0;30m"//"\e[40m"
#define ANSI_GRIS  "\e[10;10;10m"
#define ANSI_GREEN   "\e[0;32m"//"\e[42;30m"
#define ANSI_GREEN2  "\e[0;32m"//"\e[42;13m"
#define ANSI_ORANGE    "\e[0;33m"// "\e[43m"
#define ANSI_YELLOW    "\e[0;33m" //"\e[0;103;30m"
//to color the background
#define ANSI_COLOR_WHITE    "\e[47;30m"
#define ANSI_COLOR_BLUE   "\e[44m"
#define ANSI_COLOR_RED    "\e[41m"
#define ANSI_COLOR_BLACK   "\e[40m"
#define ANSI_COLOR_RESET   "\e[0m"
#define ANSI_COLOR_GRIS  "\e[10;10;10m"
#define ANSI_COLOR_GREEN   "\e[42;30m"
#define ANSI_COLOR_GREEN2  "\e[42;13m"
#define ANSI_COLOR_ORANGE    "\e[43m"
#define ANSI_COLOR_YELLOW   "\e[0;103;30m"

struct cell_preson{
int type;
enum sort_t sort;
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
   // struct set tmp_set=set_empty();
  struct set set=set_empty(); // the cell with the index 0 is the first availeble cell
  set_add(&set,0);
  //int z=0;
  for(int i=0;i<WIDTH*HEIGHT;i++){ // the loop to determine all cells that they have at least one neighbor
    //tmp_set=set;
    for(int j=0;j<set.size;j++){
      struct neighbors_t s=get_neighbors(set.t[j]);
      for(int k=0;s.n[k].d!=NO_DIR;k++)
        if(!set_find(set,s.n[k].i)) set_add(&set,s.n[k].i);
    } 

   // z=tmp_set.size;
  }

    for(int j=0;j<WIDTH*HEIGHT;j++){ //give no_cell to the cells with 0 neighbors
      if(!set_find(set,j)) world_set_sort(world,j,NO_CELL);
    } 
  }
}



// returns the neighbors with the sort=NO_SORT
struct set possible_neighbors(struct world_t* w, unsigned int idx){
  struct set set=set_empty();
  struct neighbors_t N=get_neighbors(idx);
  for(int k=0;(int)N.n[k].i!=(int)UINT_MAX;k++){ //the loop to traverse all neighbors
    unsigned int tmp=N.n[k].i;
    if(world_get(w,tmp)!=world_get(w,idx)){   //to take only the free neighbors
      set_add(&set,tmp);
    }
  }
  return set;
}

// returns the possible positions with a jump of a pawn over another other sort different to NO_SORT 
struct set  possible_jumps(struct world_t* w, unsigned int idx){
  struct set set=set_empty();
  struct neighbors_t N=get_neighbors(idx);
  unsigned int tmp;
  for(int k=0;(int)N.n[k].i!=(int)UINT_MAX;k++){ // the loop to traverse the all neighbors
    tmp=N.n[k].i;
    if(world_get_sort(w,tmp)!=NO_SORT && world_get(w,tmp)!=world_get(w,idx)){ // to take juste the pawns with a color different compared to idx
      enum dir_t dir=N.n[k].d;
      unsigned int neighbor_after=get_neighbor(tmp,dir);
      if(neighbor_after!=UINT_MAX &&  world_get_sort(w,neighbor_after)==NO_SORT ) 
      set_add(&set,neighbor_after); // the neighbor exist and available
  
    }
  }
  return set;
} 
 

//the possible moves of a pawn cell idx in the world w
struct set possible_moves(struct world_t *w,unsigned int idx,struct player other){
  struct set set=possible_neighbors(w,idx); // to take a neighbirs possible
//  struct set set2=possible_jumps(w,idx);
  /*for(int i=0;i<4;i++){ // the loop for doing a lot of jumps here the most we can do is 4 jumps
    if(!is_empty(set2)){
      for(int j=0;j<set2.size;j++){ // To verify if there are other possible jumps from the new cells
        if(!set_find(other.begining_places,set2.t[j])){ //if the cell is in the begining places of the other player, we don't search for other jump from the cell 
            world_set(w,set2.t[j],world_get(w,idx));
            world_set_sort(w,set2.t[j],world_get_sort(w,idx));
	        set2=add_sets(set2,possible_jumps(w,set2.t[j])); // to add the possible jumps of the set2.t[i] to the others jumps
            world_set(w,set2.t[j],NO_COLOR); 
            world_set_sort(w,set2.t[j],NO_SORT);
      }
      }
    }
  }
 for(int i=0;i<set2.size;i++) set_remove(&set2,idx); // for delete the possibility of returning to initail position
 */
 // return add_sets(set2,set); // add possible jumps and possible neighbors 
  return set;
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
  else{ // the player play from right to left   
    tmp=WIDTH-1;
    player->color=WHITE;
  }
  for(int i=0;i<WIDTH*HEIGHT;i++){
  player->t[i].type=1;
   player->t[i].sort=NO_SORT;
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
  //int tmp_rand=0;
 
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


//display fonction (terminal diplay for the relation 1 and 3)

void display(struct world_t *world){
  for(int i=0;i<WORLD_SIZE;i++){
    if(i%WIDTH==0 && i!=0) printf("\n");
    if(world_get_sort(world,i)==NO_CELL) printf(ANSI_COLOR_GRIS "   " ANSI_COLOR_RESET); 
    else if(world_get(world,i)==BLACK){
        switch (world_get_sort(world,i))
        {
        case TOUR:
             printf(ANSI_COLOR_GREEN2 " > " ANSI_COLOR_RESET);
            break;
        case HORSE: 
             printf(ANSI_COLOR_RED" > " ANSI_COLOR_RESET);
            break;
        case ELEPHANT:
             printf(ANSI_COLOR_ORANGE " > " ANSI_COLOR_RESET);
            break;
        case PAWN:
         printf(ANSI_COLOR_BLACK " > " ANSI_COLOR_RESET);
            break;
        case NO_SORT:
           printf(ANSI_COLOR_BLUE " > " ANSI_COLOR_RESET);
           break;
           default:
           printf(ANSI_COLOR_GREEN2"   " ANSI_COLOR_RESET);
           break;
        }
    }

    else if(world_get(world,i)==WHITE){
      switch (world_get_sort(world,i))
        {
        case TOUR:
             printf(ANSI_COLOR_GREEN " < " ANSI_COLOR_RESET);
            break;
        case HORSE:  
             printf(ANSI_COLOR_RED" < " ANSI_COLOR_RESET);
            break;
        case ELEPHANT:
             printf(ANSI_COLOR_YELLOW " < " ANSI_COLOR_RESET);
             break;
        case PAWN:  
            printf(ANSI_COLOR_WHITE " < " ANSI_COLOR_RESET);
            break;
        case NO_SORT:
          printf(ANSI_COLOR_BLUE " < " ANSI_COLOR_RESET);
          break;
          default:
          printf(ANSI_COLOR_GREEN2"   " ANSI_COLOR_RESET);
          break;
     }
    }
    else if( world_get_sort(world,i)==NO_SORT && world_get(world,i)==NO_COLOR)
      printf(ANSI_COLOR_BLUE "   " ANSI_COLOR_RESET);
  }
  printf("\n");
}
// terminal dispaly for the relation 2 (dispaly as a hexagon form)
void display_sexagon(struct world_t *world){
  for(int i=0;i<WORLD_SIZE;i++){
    if(i%WIDTH==0 && i!=0) printf("\n");
    if(world_get_sort(world,i)==NO_CELL) printf(ANSI_GRIS " " ANSI_COLOR_RESET);
    else if(world_get(world,i)==BLACK){
        switch (world_get_sort(world,i))
        {
        case TOUR:
             printf(ANSI_GREEN2 "⬣" ANSI_COLOR_RESET);
            break;
        case HORSE: 
             printf(ANSI_RED"⬣" ANSI_COLOR_RESET);
            break;
        case ELEPHANT:
             printf(ANSI_ORANGE "⬣" ANSI_COLOR_RESET);
            break;
        default:
         printf(ANSI_BLACK "⬣" ANSI_COLOR_RESET);
            break;
        }
    }

    else if(world_get(world,i)==WHITE){
      switch (world_get_sort(world,i))
        {
        case TOUR:
             printf(ANSI_GREEN "⬣" ANSI_COLOR_RESET);
            break;
        case HORSE:  
             printf(ANSI_RED"⬣" ANSI_COLOR_RESET);
            break;
        case ELEPHANT:
             printf(ANSI_YELLOW "⬣" ANSI_COLOR_RESET);
             break;
        default:  
            printf(ANSI_WHITE "⬣" ANSI_COLOR_RESET);
            break;

     }
    }
    else if(world_get(world,i)==NO_COLOR)
      printf(ANSI_BLUE "⬣" ANSI_COLOR_RESET);

  
  }
  printf("\n");

}

//sdl display 
void sdl_display(struct world_t *world){
    // all int numbers to display different colors with sdl.
  int n=0;   // color black in sdl
  int nel=256*50; //
  int nt=65536*50;
  int b=16777215; // wolor white 
  int bt=65536*255+256*100+50;
  int bel=65536*100+255*256; 
  int g=110+255*80+65536*76;   
  int bh=65536*255+140;
  int nh=65536*80+100;
  int no_cell=8066-50+65536*20;

   
  for(int i=0;i<WORLD_SIZE;i++){
    if(i%WIDTH==0 && i!=0) printf("\n");
    if(world_get_sort(world,i)==NO_CELL) printf("%d ",no_cell );
    else if(world_get(world,i)==BLACK){
        if(world_get_sort(world,i)==2)  printf("%d ",nt );
        else if(world_get_sort(world,i)==3)  printf("%d ",nel);
        else if(world_get_sort(world,i)==4)  printf("%d ",nh);
        else  printf("%d ",n );
    }
    else if(world_get(world,i)==WHITE){
        if (world_get_sort(world,i)==2) printf("%d ",bt );
        else if(world_get_sort(world,i)==3)  printf("%d ",bel);
        else if(world_get_sort(world,i)==4)  printf("%d ",bh);
        else printf("%d ",b);
    }
    else
      printf("%d ",g );
    
  }
  printf("\n");
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




// play function with rand moves
void play(struct player *player1,struct world_t *world,struct player *other){
  if(player1->current_places.size!=0){  
  int b=rand()%player1->current_places.size; // to choose randomply one index from the current_places
    int a=player1->current_places.t[b]; 
    int tmp=0;
    int move=0; // to choose randomly the move
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
            set=possible_moves(world,a,*other);            
      }

      if(set.size!=0 ){ // if possible moves not empty
         move=rand()%set.size;
        }
      //for the escape and the case is available
      if(player1->t[a].type==PRESONNIERE && world_get_sort(world,a)==NO_SORT){
          printf("-----------------------------------------------------------------------------------------------------------------------\n");    
          world_set_sort(world,a,player1->t[a].sort);
          world_set(world,a,player1->color);
          player1->t[a].type=1;
          ///////////////////printf("hadi li ghanji fiha sort dyalha ha kiwlat  %d rand les hommes\n",world_get_sort(world,a));
         tmp=3;
        }
       //for the escape and the case is not available we search of another cell
        else if(player1->t[a].type==PRESONNIERE && world_get_sort(world,a)!=NO_SORT){
          b=(b+1)%player1->current_places.size; // to choose randomply one index from the current_places
          a=player1->current_places.t[b];  
         ///////////////////////////// printf("on espère que sera changer\n"); 
          loop++;
        }
        //for not take cells in the begining places of any players
        else if( set_find(other->begining_places,a)!=0 || world_get_sort(world,a)==NO_SORT || ((set_find(player1->begining_places,set.t[move])||set_find(other->begining_places,set.t[move]) ) && world_get_sort(world,set.t[move]!=NO_SORT)) ){
          b=(b+1)%player1->current_places.size; // to choose randomply one index from the current_places
          a=player1->current_places.t[b];  
          ////////////////////////printf("walo\n");
          loop++;
        }
        
        else {
          printf("Ta w7da ya3ni nda o sort li 3ndi hya %d o color %d knt f lblasa%d ghanwli l %dohya db %d type %d\n",player1->t[a].sort,player1->color,a,set.t[move],world_get_sort(world,a),player1->t[a].type);
          tmp=2;
        }
        //to avoid infinite loop 
        if(loop>=player1->current_places.size+10){
          tmp=3;
        }

    }
    
    if(tmp==2){
      //To capture the opponent
       if(world_get_sort(world,set.t[move])!=NO_SORT ){
              other->t[set.t[move]].type=PRESONNIERE;       
         }  

         player1->t[set.t[move]].sort=player1->t[a].sort;
         player1->t[a].sort=NO_SORT;
         player1->t[a].type=1;
         player1->t[set.t[move]].type=1;
         player1->current_places.t[b]=set.t[move];
         
         world_set(world,set.t[move],player1->color);
         
         world_set_sort(world,set.t[move],world_get_sort(world,a));
         world_set_sort(world,a,NO_SORT);
         world_set(world,a,NO_COLOR);
       }   
  }
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
  //printf("⬣\n") ;  
  
  printf("%d %d\n",WIDTH,HEIGHT);
  printf("#\n"); 
 // printf("-------------------------------%d\n",type_plateau);
  struct world_t* world=world_init(); //initialize the world
  initilize_plateau(world,type_relation());
  struct player player1;
  struct player player2;
  int random_player=rand()%2+1; //choosing the player who starts randomly
  int other_player=(random_player%2)+1; // random player and other player must be one of them 1 and the other 2
  int elephant_number=0; // the number of ELEPHANT cells for each player
  int horse_number=0; // the number of HORSE cells for each player
  int tour_number=0;  // the number of TOUR cells for each player 
  if(type_relation()==3) horse_number=0;
  
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
    play(&player1,world,&player2);
display(world);
printf("player %d\n",player1.color);
    play(&player2,world,&player1);
   //if(get_neighbors_seed()==2) display_sexagon(world);
  //else{
    display(world);  
    printf("\n");     // for terminal display
    //sdl_display(world);   // sdl diplay
  //}


    if(win_game(player1,player2,type)){
      printf("player1 gagne\n");
      i=iterations+1;
    }
    else if(win_game(player2,player1,type)){
      printf( "player2 gagne\n");
      i=iterations+1;
    }
    printf("\n");

  }
  if(i==iterations) printf("Match nul\n"); 

  return 0;

}

   
