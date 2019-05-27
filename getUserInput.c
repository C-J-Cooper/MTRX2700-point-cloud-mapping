/**
* @file getUserInput.c
* @author: Wednesday Group 1
* @date June, 2018
* @brief Reads the user input from serial
*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "functions.h"
#include "sci1.h"

#define SIZE 100

/* helper functions */
char readChar(void);
int str2num(char *inputStr, int length);


extern long int minHead, maxHead, minTilt, maxTilt, dMThetaTilt, dMThetaHead;
extern int numSamp, freqSamp;         

/**
 * \fn void inputConfigBoard(void)
 * \breif Reads the the user input from serial.
 * This funtion prompts the user to enter a value for each input variable.
 * Once user has entered a value the function modifies the value of the global variables which store the user input.
 */
void getUserInput(){
  
  // read the azimuth
  do{
    
    minHead = getInt("Enter Minimum Azimuth (deg 20<x<160) \r\n", 40);
     _asm NOP;   // dummy statement  
    
  }while(minHead < 20 || minHead > 160); 
  
    do{
    
    maxHead = getInt("Enter Maximum Azimuth (deg 30<x<160) \r\n", 40);
     _asm NOP;   // dummy statement  
    
  }while(maxHead < 30 || maxHead > 160 || maxHead < minHead);
  
  
  // read azimuth step change resolution
  do{
    
    dMThetaHead = getInt("Enter azimuth step change in degrees x 100. e.g. 100 = 1 degree, 250 = 2.5 degrees\n\r", 85);
      
  }while(dMThetaHead < 10 || dMThetaHead > 500); //at the moment min is 0.1 degree and max is 5 degrees  
  
  
  // read the elevation 
  do{
    
    minTilt = getInt("Enter Minimum Elevation (deg -60<y<60) \r\n",42);
     _asm NOP;   // dummy statement
    
  }while(minTilt < -60 || minTilt > 60);
  
  do{
    
    maxTilt = getInt("Enter Maximum Elevation (deg -60<y<60) \r\n",42);
     _asm NOP;   // dummy statement
    
  }while(maxTilt < -60 || maxTilt > 60 || maxTilt < minTilt);
  
  
  // read the elevation step change resolution
  do{

    dMThetaTilt = getInt("Enter elevation step change in  degrees x 100. e.g. 100 = 1 degree, 250 = 2.5 degrees\n\r", 88);
      
  }while(dMThetaTilt < 10 || dMThetaTilt > 500);
  
  do{
    
    numSamp = getInt("Enter number of samples (1<x<30):\r\n",36);
     _asm NOP;   // dummy statement
      
  }while(numSamp < 1 || numSamp > 31);
  
  do{
    // gyro has maz frequency of 100 Hz. Setting to 80 Hz for saftey
    freqSamp = getInt("Enter the sample frequency in Hertz.(1<freq<10)\n\r", 50);
      
  }while(freqSamp < 1 || freqSamp > 10); 
  
}

int getInt(char text[], int num){    //Funtion which get the input from terminal

  char inputStr[8];
  char tempInput;
  int inputStrInx;
  int inputNum;

  // prompt user for an input
  SCI1_OutString(text);		

   // read the input
  inputStrInx = 0;
  tempInput = readChar();
  while(tempInput != '\r'){
    
    inputStr[inputStrInx] = tempInput;
    ++inputStrInx;
    tempInput = readChar();
  
  }
  
  inputStr[inputStrInx] = 0;
  tempInput = 0;
  // convert the string to an integer
  inputNum = str2num(inputStr, inputStrInx);
  _asm NOP;   // dummy statement
  
  
  return(inputNum);
}

/*Function whichs converts from character type to integer type*/
/**
 * \fn int str2num(char *inputStr, int length)
 * \breif Converts an string to an integer.
 * \param char *inputStr The string to be converted to a number
 * \param int length Length of the string
 */
int str2num(char *inputStr, int length){  
  int num;
  int mult;
  int i;
    
  num = 0;
  i = length-1;
  mult = 1; 
  while(i>=0){
  
  if(inputStr[i]== '-'){
   num = -1*num;
   break;
    
  }
  
  num = num + mult*(inputStr[i]-0x30);
  mult = mult*10;
  --i;
  
  }
  
  return(num);
  
}


/**
 * \fn void inputConfigBoard(void)
 * \breif Reads a character from serial input
 */
char readChar(void){

  while(!(SCI1SR1 & 0x20)){
  
  }// wait for data register to be full
  
  return(SCI1DRL);           // return the character
  
}