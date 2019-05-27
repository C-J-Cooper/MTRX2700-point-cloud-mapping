/**
* @file delay1ms.c
* @author: Wednesday Group 1
* @date June, 2018
* @brief Creates 1 millisecond delay
*/
#include <hidef.h>            /* common defines and macros */
#include "derivative.h"       /* derivative-specific definitions */

/**
 * \fn void delay1ms(unsigned dt)
 * \breif Creates a delay in milliseconds
 * \param unsigned dt The number of milliseconds to delay by
 */
void delay1ms(unsigned dt){

  TC4 = TCNT + (word)1500*dt; // add of offset to get desired delcay
  
  while(!(TFLG1 & 0x10));   // poll and wait for successful output compare
}