/**
* @file sendStrLCD.c
* @author: Wednesday Group 1
* @date June, 2018
* @brief Sends a string to serial output
*/
#include <hidef.h>            /* common defines and macros */
#include "derivative.h"       /* derivative-specific definitions */
#include "functions.h"            /* delay functions */

void sendCharLCD(char ch);

/**
 *	\fn void sendStrLCD(char *string, int stringLen)
 *  \breif This function sends a string the LCD
 *	\param char *string The string to be sent the LCD
 *  \param int stringLen The length of the string to send to the LCD
 
 *  This function was written using the example in "The HCS12 / 9S12: An Introduction to Software and Hardware Interfacing"
 *  By Han-Way Huang,as a reference
 */
void sendStrLCD(char *string, int stringLen){
  int stringInx;
  
  stringInx = 0;
  
  while(stringInx<stringLen-1){
      
    sendCharLCD(string[stringInx]);
    ++stringInx;  
  }
  
}

/**
 *	\fn void sendCharLCD(char ch)
 *  \breif This function writes a character the LCD
 *	\param char ch The character to be sent to the LCD
 
 *  This function was written using the example in "The HCS12 / 9S12: An Introduction to Software and Hardware Interfacing"
 *  By Han-Way Huang,as a reference
 */
void sendCharLCD(char ch){

  /* declaring the local varibles */
  /**
   * \var char chL4
   * \breif Stores the lower 4 bits of the character
   */
   /**
   * \var char chL4
   * \breif Stores the upper 4 bits of the character
   */
  char chL4;             // lower 4 bits of the character
  char chU4;             // upper 4 bits of the character

  PORTK |= 0x01;        // select the data register
  PORTK |= 0x02;        // pull the E pin to high
  
  chU4 = ch & 0xF0;     // get the upper 4 bits of the cmd
  chU4 = chU4 >> 2;     // shift the upper 4 bits to match the data pins for the LCD 
  PORTK = chU4 | 0x03;   // need to keep the E and the RS pin high
    
  _asm NOP;             // wait for LCD to receive the data
  _asm NOP;
  _asm NOP;
  
  PORTK &= ~0x02;       // pull the E pin low


  chL4 = ch & 0x0F;     // get the lower 4 bits of the cmd
  chL4 = chL4 << 2;     // shift the lower 4 bits to match the data pins for the LCD
  
  PORTK = chL4 | 0x03;   // need keep the E and RS pin high
    
  _asm NOP;             // wait for LCD to receive the data
  _asm NOP;
  _asm NOP;
  
  PORTK &= ~0x02;       // pull the E pin low
  
  delay2us(25);         // delay by 50 microseconds for LCD to complete operation Insgtructions

}

