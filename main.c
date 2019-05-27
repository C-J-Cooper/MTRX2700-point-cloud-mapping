/**
* @file main.c
* @author: Wednesday Group 1
* @date June, 2018
* @brief This is the main file for the TBMS mapping system
*
* \mainpage Description
* This program scans a small 3x3x3 meter area using a pan and tilt unit and collects the sensor data.
* The sensor data can then be used in a seperate program to generate a point cloud map of the area scanned.
* The resolution, accuracy, sample frequency and mapping area is configurable by the user upon starting the program.
* Click on the 'files' tab in the top
* panel to nagivate the functions and variables that are used in the mapping system program.
*/
#include <hidef.h>      /* common defines and macros */
#include <math.h>
#include <stdlib.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "functions.h"
#include "iic.h"  
#include "pll.h"
#include "sci1.h"
#include "l3g4200.h"  // register's definitions    ( not used by ed )

       
void ang_str(int ang, char *str, int digits);
void servoMapping (void);
unsigned median(int n, int x[]); // to find the median

/**
* \var long int minHead
* \breif Stores the minimum azimuth of the mapping area. Value configed by the user.
*/
/**
* \var long int maxHead
* \breif Stores the maximum azimuth of the mapping area. Value configed by the user.
*/
/**
* \var long int minTilt
* \breif Stores the minimum angle of elevation of the mapping area. Value configed by the user.
*/
/**
* \var long int maxTilt
* \breif Stores the maximum angle of elevation of the mapping area. Value configed by the user.
*/
/**
* \var long int dMThetaTilt
* \breif Controls the elevation step change between samples. Value configured by the user.
*/
/**
* \var long int dMThetaHead
* \breif Controls the azimuth step change between samples. Value configured by the user.
*/
/**
* \var long int currentHead
* \breif Used by the servomapping function. Contains the current azimuth of the servos.
*/
/**
* \var long int currentTilt
* \breif Used by the servomapping function. Contains the current angle of elevation of the servos.
*/
long int minHead, maxHead, minTilt, maxTilt, dMThetaTilt, dMThetaHead, currentHead, currentTilt;
/**
* \var int numSamp
* \breif Determines how many distance samples to take at each orientation. Value is configured by the user.
*/
/**
* \var int freqSamp
* \breif Saves the sample frequency. Value is configured by the user.
*/
/**
* \var int direction
* \breif Controls whether the PTU is scanning upward or downward.
*/
/**
* \var serialOrBoard
* \breif Saves the result of the user's response when asked if they are entering configuration on the keypad or over serial.
*/
int numSamp, freqSamp, direction, serialOrBoard;
/**
* \var float periodSamp
* \breif Variable to store the sample period.
*/
float periodSamp;


// LIDAR GLOBALS
// global varaibles, used in displaying distance on the 7 segment display

/**
* \var unsigned sevenSegInx
* \breif Used to control which 7 segment display is active.
*/
unsigned sevenSegInx;
/**
* \var unsigned distGlobal
* \breif Stores the median distance of all the samples.
*/
unsigned distGlobal;
/**
* \var unsigned distGlobalTemp
* \breif Used to display each digit of the current distance on the 7 segement display.
*/
unsigned distGlobalTemp;


/**
* \var unsigned currentDistShort[50] 
* \breif An array to store the value of all the distance samples.
*/
unsigned currentDistShort[50];
/**
* \var unsigned currentHeadShort 
* \breif Stores the current azimuth of the servos.
*/ 
unsigned currentHeadShort;
/**
* \var unsigned currentTiltShort 
* \breif Stores the current angle of elevation of the servos.
*/
unsigned currentTiltShort;   // used to send tilt and heading to serial output
/**
* \var unsigned sampleInx
* \breif Tracks which distance sample is currently being measured.
*/ 
unsigned sampleInx;      // used to get all the samples

/**
* \var char headingString[5]
* \breif The current heading as a string of characters. Used to display heading to the LCD.
*/
char headingString[5];
/**
* \var char tiltString[5]
* \breif The current elevaiton as a string of characters. Used to display elevation to the LCD.
*/
char tiltString[5];


/* -----------------------------
 * MAIN PROGRAM
 * -----------------------------*/
/**
* \vfn void main(void)
* \breif The main function of mapping program.
*  This function takes in user configuration over the keypad or over serial scans the dersired area.
*  Mapping data is sent back to the PC over serial in real time.
*/
void main(void) {
  
  uint8_t D2W, D3R, WHO, who;
  int  res1, res2,  res3, *p, integer = 0, periodIndex;
  float acc;


 	  
  // setting default values for configuration
  minHead = 80000;
  maxHead = 120000;
  minTilt = 70000;
  maxTilt = 130000;
  dMThetaTilt = 2000;
  dMThetaHead = 2000;
  numSamp = 20;
  currentHead = minHead;
  currentTilt = minTilt;
  direction = 0;
  freqSamp = 200;
  serialOrBoard = 0;
  
  PLL_Init();  // make sure we are runnign at 24 Mhz
  mainSetup();
  EnableInterrupts;

//This program will send the gyro, accelerometer and magnetometer data
// to a serial port
// You can connect the serial port, set it to 9600 bauds 

  SCI1_Init(BAUD_9600);   // capped at 9600, if PLL inactive (4 MHz bus)
 
  SCI1_OutString("Program Starting ");      // should display this
 
  inputConfigBoard();
  if (serialOrBoard){
    getUserInput();
  }
 
 
 // convert the raw input to format used by the control modules
  minHead = minHead*1000;
  maxHead = maxHead*1000;
  minTilt = (minTilt+90)*1000;
  maxTilt = (maxTilt+90)*1000;
  dMThetaTilt = dMThetaTilt*10;
  dMThetaHead = dMThetaHead*10;
  currentHead = minHead;
  currentTilt = minTilt;
  freqSamp = freqSamp;		// still not used

  distGlobal = 0;
  sevenSegInx = 4;
  TC5 = TCNT + 30;      // add small off set to create cascade of interrupts

	SCI1_OutString("$\r\n");	// use '$' sign to mark begining of the mapping data 

  

  while(1){

    servoMapping();
    
    sampleInx = 0;
    while(sampleInx < numSamp){
      
      currentDistShort[sampleInx] = distanceLIDAR();
      
      ++sampleInx;
    }
    distGlobal = median(numSamp, currentDistShort); // only updating global variable once
    SCI1_OutUDec((unsigned ) distGlobal);
    SCI1_OutString(",");
    
    SCI1_OutUDec((unsigned ) currentHeadShort);
    SCI1_OutString(",");
    SCI1_OutUDec((unsigned ) currentTiltShort);
    SCI1_OutString("\n");
    
       
    LCDcmd(0x01);         // clear display and return to the home position
    delay1ms(2);          // delay by 2 milliseconds for the clear display command to complete
    ang_str(currentHeadShort, headingString, 4); 
    sendStrLCD("Azimuth:", 9);
    sendStrLCD(headingString, 6);
    LCDcmd(0xC0);
    delay1ms(2);
    sendStrLCD("Elevation:", 11);
    sendStrLCD(tiltString, 6);

   
    if(currentHead > maxHead) { 
   	  SCI1_OutString("#\r");	// use '#' sign to mark begining of the mapping data
   	  break; 	
    }
    
    // 
    periodSamp = 1000/freqSamp;
    if (periodSamp-2*numSamp-50 > 0){
      while (periodIndex < periodSamp - numSamp-50){
         delay1ms(1); 
         periodIndex += 1;
      }
      periodIndex = 0;
    }

  }
   
   while(1){
   	// loop forever
   }
 
}

/**
* \fn void servoMapping (void)
* \breif Controls the position of the servo motors during mapping
*/
void servoMapping(void){
  
   if(currentHead <= maxHead){
     
     // Rotate lidar up
     if(currentTilt <= maxTilt && direction == 0){        
        servoControl(currentTilt, currentHead);
        ang_str(currentTilt/100-900, tiltString, 4);
        currentHeadShort = currentHead/100;
        currentTiltShort = currentTilt/100;
        delay2us(10);
        currentTilt += dMThetaTilt;
     }else if(currentTilt >= maxTilt  && direction == 0){
      currentHead += dMThetaHead;
      currentTilt -= dMThetaTilt;
      servoControl(currentTilt, currentHead);
      currentHeadShort = currentHead/100;
      currentTiltShort = currentTilt/100;
      ang_str(currentTilt/100-900, tiltString, 4);
      direction = 1;
     }
     
     // Rotate lidar down
     if(currentTilt >= minTilt && direction == 1){
       servoControl(currentTilt, currentHead);
       ang_str(currentTilt/100-900, tiltString, 4);
       currentHeadShort = currentHead/100;
       currentTiltShort = currentTilt/100;
       delay2us(10);
       currentTilt -= dMThetaTilt;
      
     }else if(currentTilt <= minTilt  && direction == 1){
      currentHead += dMThetaHead;
      currentTilt += dMThetaTilt;
      servoControl(currentTilt, currentHead);
      currentHeadShort = currentHead/100;
      currentTiltShort = currentTilt/100;
      ang_str(currentTilt/100-900, tiltString, 4);
      direction = 0;
     }     
  }
  
}

/**
* \fn interrupt 13 void timer5_ISR(void)
* \breif Interrupt subroutine which displays the current distance to the seven segment display. 
*/
interrupt 13 void timer5_ISR(void){
  int DISPLAY_CODES[]= {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,\
0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
  unsigned digit;


  if(sevenSegInx ==4){
  
  distGlobalTemp = distGlobal/15;
  
  digit = distGlobalTemp%10;
  
  PTP = 0xF;                    // disable all LEDs to prevent ghosting
  PORTB = DISPLAY_CODES[digit];   
  PTP = 0x7;    // enable right most display
  
  TC5 = TCNT + 3*50;    // delay by 100 micro seconds
  
  distGlobalTemp = distGlobalTemp/10;
  
  }else if(sevenSegInx == 3){
  
  digit = distGlobalTemp%10;
  
  PTP = 0xF;                    // disable all LEDs to prevent ghosting
  PORTB = DISPLAY_CODES[digit]; 
  PTP = 0xB;    // enable right most display
  
  distGlobalTemp = distGlobalTemp/10;
  
  TC5 = TCNT + 3*50;    // delay by 100 micro seconds
  }else if(sevenSegInx == 2) {
  
  digit = distGlobalTemp%10;
  
  PTP = 0xF;                    // disable all LEDs to prevent ghosting
  PORTB = DISPLAY_CODES[digit]+0x80; 
  PTP = 0xD;                    // enable right most display
  
  distGlobalTemp = distGlobalTemp/10;
    
  TC5 = TCNT + 3*50;            // delay by 100 micro seconds
  }else if(sevenSegInx == 1) {
  
  digit = distGlobalTemp%10;
  
  PTP = 0xF;                    // disable all LEDs to prevent ghosting
  PORTB = DISPLAY_CODES[digit]; 
  PTP = 0xE;                    // enable right most display
  
  distGlobalTemp = distGlobalTemp/10;
    
  TC5 = TCNT + 3*50;            // delay by 100 micro seconds
  sevenSegInx = 5;
  } 
  
  --sevenSegInx;
   
}

/**
* \fn unsigned median(int len, int dist[])
* \breif Returns the median value in an unordered array on integers.
* 
* \param int dist[] The array containing all the distances.
* \param int len Length of the input array.
*/
unsigned median(int len, int dist[]) {
    
    unsigned temp;
    int i, j;
    
    /* sorting the distance array into ascending order */
    i=0;
    while(i<len-1){
        j = i+1;
        
        while(j<len) {
            if(dist[j] < dist[i]) {
                
                temp = dist[i];
                dist[i] = dist[j];
                dist[j] = temp;
                
                ++j;
            }
        }
    }

	/* returning the value in the middle of the sorted distance array*/
    if(len%2==0) {

        return((dist[len/2] + dist[len/2 - 1]) / 2);
    
    } else {

        return dist[len/2];
    }
}

/**
* \fn void ang_str(int ang, char *str, int digits)
* \breif This function converts the bearing and tilt to a string. 
* 
* \param int ang The input elevation/azimuth
* \param char *str The string to save the result of the number to string conversion
* \param int digits Number of digits in the input elevation/azimuth
*/
void ang_str(int ang, char *str, int digits){							
  int i = digits;
  
  if(ang < 0){											    /* If the angle is negative, set the first index of string */
    *str = 45;												/* to be ASCII -.                                          */
  } 
  else {
    *str = 32;												/* Otherwise set it to be ASCII space.      			   */
  }
  while (i>0) {												/* For the remaining 3 indices, convert the unsigned number*/
  *(str+i) = abs(ang%10)+48;								/* using modulus and adding 48 to convert to ASCII         */
  ang /= 10;
  i--;
  }
  
}