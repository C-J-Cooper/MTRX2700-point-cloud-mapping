/**
* @file mainSetup.c
* @author: Wednesday Group 1
* @date June, 2018
* @brief Calls the setup functions for all the subsystems.
*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

void configLCD(void);
void timerSetup(void);
void setupPWM(void);
void setupSerial(void);
void setupInterrupt(void);
void sevenSegSetup(void);

/** 
 * \fn void mainSetup(void)
 * \brief Calls the setup functions for all the subsystems.
 *
 */
void mainSetup(void){

 timerSetup();
 configLCD();
 setupPWM();
 setupSerial();
 setupInterrupt();
 sevenSegSetup(); 
  
}

/** 
 * \fn void configLCD(void)
 * \brief Configures the LCD so it can receive commands and display characters. 
 *
 */
void configLCD(void){
  DDRK = 0xFF;          // set pork k as an output
  delay1ms(100);         // wait 100 milliseconds for LCD to be ready
  LCDcmd(0x28);         // set 4 data bits, two lines and 5x8 font
  LCDcmd(0x0F);         // turn on the cursor
  LCDcmd(0x06);         // move cursor to the right
  LCDcmd(0x01);         // clear display and return to the home position
  delay1ms(2);          // delay by 2 milliseconds for the clear display command to complete
}

/** 
 * \fn void timerSetup(void)
 * \brief initializes the timer subsystem on the Dragon 12.
 *
 */
void timerSetup(void){

  TSCR1 = 0x90; // enable timer function and enables fast flag clear
  TSCR2 = 0x04; // Set prescaler to 16
  TIOS  = 0x30; // Set channel 4 and 5 to output compare, channel 1 to input compare
  
  _asm sei;
  TIE = 0x20;  // enable interrupts from TC5
  _asm cli;

}

/** 
 * \fn void setupPWM(void)
 * \brief initializes the PWM subsystem. The PWM system controls the position of the servos.
 *
 */
void setupPWM(void){

  PWMCTL = 192;
  PWMCLK = 0;
  PWMPOL = 160;
  PWMPRCLK = 0x33;
  PWMCAE = 0;
  PWMSCLA = 0;
  PWMSCLB = 0;
  PWMPER4 = 0xEA;
  PWMPER5 = 0x60;
  PWMPER6 = 0xEA;
  PWMPER7 = 0x60;
  PWMDTY4 = 0x11;
  PWMDTY5 = 0x94;
  PWMDTY6 = 0x11;
  PWMDTY7 = 0x94;
  PWME = 160;  
  
}

/** 
 * \fn void setupSerial(void)
 * \brief Sets up serial communications.
 *
 */
void setupSerial(void){

  SCI1BDH = 0;
  SCI1BDL = 156;
  SCI1CR1 = 0x4C;
  SCI1CR2 = 0x0C;  
  
}

/** 
 * \fn void setupInterrupt(void)
 * \brief Enables timer interrupts from TC6
 *
 */
void setupInterrupt(void){
 
  _asm sei;
  SCI1CR2 |= 0x20;
  _asm cli;  
  
}

/** 
 * \fn void sevenSegSetup(void)
 * \brief initializes the 7 segment display
 *
 */
void sevenSegSetup(void){
  
  DDRB = 0xFF;    // data bus for 7 seg and LEDs
  DDRP = 0xFF;    // control bus for 7 segment
  DDRJ = 0xFF;    // to enable the LEDs
  PTJ = 0x00;     // to enable the LEDs
    
}