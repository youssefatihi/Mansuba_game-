#include <stdio.h>
#include <stdlib.h>
#include "geometry.h"



const char* place_to_string(enum color_t c, enum sort_t s){
  if(c==0 && s>=0) return "Empty_place"; 
  if(c==1 && s>=0) return "Black_place";
  else return "White_place";


}


const char* dir_to_string(enum dir_t d){
  switch((int)d){
  case 0:
    return "NO_DIR";
  case 1:
    return "EAST";
  case 2:
    return "NEAST";
  case 3:
    return "NORTH" ;
  case 4:
    return "NWEST";
  case -1:
    return "WEST";
  case -2:
    return "SWEST";
  case -3:
    return "SOUTH";
  case -4:
    return "SEAST";
  }
  return "erreur"; 

}
