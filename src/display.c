#include <stdio.h>
#include <stdlib.h>
#include "display.h"


// Constants for the terminal display 
//to color the writing
#define ANSI_WHITE     "\e[0;37m"
#define ANSI_BLUE   "\e[0;34m"
#define ANSI_RED    "\e[0;31m"
#define ANSI_BLACK   "\e[2;30m"
#define ANSI_GRIS  "\e[10;10;10m"
#define ANSI_GREEN   "\e[0;32m"
#define ANSI_GREEN2  "\e[2;32m"
#define ANSI_ORANGE    "\e[2;33m"
#define ANSI_YELLOW    "\e[0;33m" 
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
           printf(ANSI_COLOR_BLUE "   " ANSI_COLOR_RESET);
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
          printf(ANSI_COLOR_BLUE "   " ANSI_COLOR_RESET);
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
