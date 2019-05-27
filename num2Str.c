#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

int num2str(int timeHigh, char* distance){

  int length;     // length of output string
  int distanceInx = 0;
  
  /* first need to find the size of number */
  if (timeHigh == 0){
    length = 1;
  }else if(timeHigh >= 10000){
    length = 5;  
  }else if(timeHigh >= 1000){
    length = 4;
  }else if(timeHigh >= 100){
    length = 3;
  }else if(timeHigh >= 10){
    length = 2;
  }else if(timeHigh >= 1){
    length = 1;
  }else{
    length = 0; //something has gone wrong if length is equal to zero
  }
  distanceInx = length;
  distance[distanceInx] = 0;     // the string should end in a null character
 
 --distanceInx;
 
 
 while(distanceInx >= 0){
  distance[distanceInx] = timeHigh%10 + 0x30;   // get the ascii code of the number and save to string
  timeHigh = (int)timeHigh/10;                  // divide number by 10 and truncate any remainder
  --distanceInx;
 }
   
 return length+1;
  
}