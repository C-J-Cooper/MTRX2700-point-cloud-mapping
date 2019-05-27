/**
* @file delay2us.c
* @author: Wednesday Group 1
* @date June, 2018
* @brief creates 2 micro second delay
*/
#include <hidef.h>            /* common defines and macros */
#include "derivative.h"       /* derivative-specific definitions */

/**
 * \fn void delay2us(unsigned dt)
 * \breif Creates a delay in microseconds
 * \param unsigned dt Number of 2 microsecond to delays
 * e.g. Input of 2 creates delay of 4 microsecs.  
 */
void delay2us(unsigned dt){

  TC4 = TCNT + (word)3*dt;
  
  while(!(TFLG1 & 0x10));
  
}