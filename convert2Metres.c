#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

/* function used to convert the reading from the LIDAR to a string in meters
 to be displayed on the LCD 
 Will be used for the LIDAR demonstration
 */
int convert2Metres(unsigned short timeHigh, char* distance){
  
  int length;     // length of output string
  int i;
  int distanceInx;
  
  timeHigh = (unsigned short)timeHigh/15;
  
 
  
  /* first need to find the size of number */
  if(timeHigh >= 10000){
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
 
 distanceInx = length + 1;
 distance[distanceInx] = 0;     // the string should end in a null character
 
 --distanceInx;
 
 i = 2;
 while(distanceInx >= 0){
  if( i==0 ){
    distance[distanceInx] = 0x2E;               // put in a decimal point
  } else{
    
  distance[distanceInx] = timeHigh%10 + 0x30;   // get the ascii code of the number and save to string
  timeHigh = (unsigned short)timeHigh/10;                  // divide number by 10 and truncate any remainder

 }
 
  --i;
  --distanceInx;
 }
 return length+2;  
  
}