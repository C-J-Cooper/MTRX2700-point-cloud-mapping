#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

void timerSetup(void){

  TSCR1 = 0x90; // enable timer function and enables fast flag clear
  TSCR2 = 0x04; // Set prescaler to 16
  TIOS  = 0x30; // Set channel 4 and 5 to output compare, channel 1 to input compare
  
  _asm sei;
  TIE = 0x20;  // enable interrupts from TC5
  _asm cli;


}