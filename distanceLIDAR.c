/**
* @file distanceLIDAR.c
* @author: Wednesday Group 1
* @date June, 2018
* @brief Calculates the distance measured by the LIDAR subsystem
*/
#include <hidef.h>            /* common defines and macros */
#include "derivative.h"       /* derivative-specific definitions */

/* function to get the distance from the LIDAR sensor ---------------------*/
/**
 * \fn unsigned short distanceLIDAR(void)
 * \breif Function to get the distance from the LIDAR sensor
 * This function measures the pulse width of the PWM signal output by the LIDAR subsystem
 */ 
unsigned short distanceLIDAR(void){
  
  /**
   * \var unsigned short risingEdge
   * \breif Value of timer counter at the rising edge
   */
  /**
   * \var unsigned short fallingEdge
   * \breif Value of timer counter at the falling edge
   */
  /**
   * \var unsigned short timeHigh
   * \breif Length of the pulse width in clock ticks.
   */
  unsigned short risingEdge, fallingEdge, timeHigh;
  
  TCTL4 = 0x04;           // prepare to capture the rising edge from TC1
  
  while(!(TFLG1 & 0x02)){  
    // wait for the arrival of the rising edge
  }
  
  risingEdge = TC1;
  
  TCTL4 = 0x08;           // prepare to capture the rising edge from TC1
  
  while(!(TFLG1 & 0x02)){
    // wait for the arrival of the rising edge 
  }
  
  fallingEdge = TC1;
  
  if(fallingEdge > risingEdge){
    timeHigh = fallingEdge - risingEdge;
   } else if(fallingEdge < risingEdge){
    timeHigh = fallingEdge + 65535 - risingEdge;
   } else{
     // something has gone wrong, times should not be the same
     timeHigh = 0;
   }
   
   timeHigh = timeHigh - 150;  // removing the bias from the distance sensor  
    
   if(timeHigh > 65380){   // if distance is negative, set to zero
    timeHigh = 0;
   }
   
   return timeHigh;
  
}