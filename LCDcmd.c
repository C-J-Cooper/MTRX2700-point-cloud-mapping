/**
* @file LCDcmd.c
* @author: Wednesday Group 1
* @date June, 2018
* @brief Writes commands to the LCD subsystem
*/
#include <hidef.h>            /* common defines and macros */
#include "derivative.h"       /* derivative-specific definitions */
#include "functions.h"            /* delay functions */

/**
 *	\fn void LCDcmd(char cmd)
 *  \breif This function writes commands to the LCD subsystem
 *	\param char cmd The command to be written to the LCD
 
 *  This function was written using the example in "The HCS12 / 9S12: An Introduction to Software and Hardware Interfacing"
 *  By Han-Way Huang, pg 333 as a reference
 */
void LCDcmd(char cmd){
  
  /* declaring the local varibles */
 /**
   * \var char cmdL4
   * \breif Stores the lower 4 bits of the command
   */
 /**
   * \var char cmdL4
   * \breif Stores the upper 4 bits of the command
   */
  char cmdL4;            // lower 4 bits of the command
  char cmdU4;            // upper 4 bits of the command
  
  
  PORTK &= ~0x01;       // select the intrsutction register by clearing the RS pin
  PORTK |= 0x02;        // disable signal by clearing the E pin
  
  cmdU4 = cmd & 0xF0;   // get the upper 4 bits of the cmd
  cmdU4 = cmdU4 >> 2;   // shift the upper 4 bits to match the data pins for the LCD 
  PORTK = cmdU4 | 0x02; // output the upper 4 bits. need to keep the E pin high
  
  _asm NOP;             // wait for LCD to receive the data
  _asm NOP;
  _asm NOP;
  
  PORTK &= ~0x02;       // pull the E pin low
  
  cmdL4 = cmd & 0x0F;   // get the lower 4 bits of the cmd
  cmdL4 = cmdL4 << 2;   // shift the lower 4 bits to match the data pins for the LCD
  
  PORTK = cmdL4 | 0x02; // send lower 4 bits. need keep the E pin high
  
  _asm NOP;             // wait for LCD to receive the data
  _asm NOP;
  _asm NOP;

  PORTK &= ~0x02;       // pull the E pin low
    
  delay2us(25);         // delay by 50 microseconds for LCD to complete operation Insgtructions
    
}