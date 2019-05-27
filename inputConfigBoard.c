/**
* @file inputConfigBoard.c
* @author: Wednesday Group 1
* @date June, 2018
* @brief Reads the user input from the Keypad
*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "functions.h"


extern long int minHead, maxHead, minTilt, maxTilt, dMThetaTilt, dMThetaHead;
extern int numSamp, freqSamp, serialOrBoard; 

/**
* \var unsigned char column 
* \breif Used to scan each coloumn
*/
/**
* \var unsigned char row 
* \breif Used to scan each row
*/
unsigned char column,row;
/**
* \var char *menuString[9]
* \breif An array of strings to store the user intructions
*/
char *menuString[9];
/**
* \var int inputIndex
* \breif Index to keep track of which configuration value is being entered.
*/
int inputIndex;
/**
* \var int inputs[9]
* \breif Array to save the value of the user input
*/
int inputs[9];

// *************MAIN*******************************/
/**
 * \fn void inputConfigBoard(void)
 * \breif Reads the the user input from the keypad.
 * This function scans the rows and coloumns on the keypad to determine which button is being pressed.
 * Once user has entered a values the function modifies the value of the global variables which store the user input.
 */
void inputConfigBoard(void){
   char numString[4];
   int lenString;
   
   menuString[0]="minAzim: ";
   menuString[1]="maxAzim: ";
   menuString[2]="stepAzi: ";
   menuString[3]="minElev: ";
   menuString[4]="maxElev: ";
   menuString[5]="stepEle: ";
   menuString[6]="sampNum: ";
   menuString[7]="sampFrq: ";
   menuString[8]="Serial(1)/Board(2)";
   
   inputs[0] = 19;
   inputs[1] = 160;
   inputs[2] = 100;
   inputs[3] = -60;
   inputs[4] = 60;
   inputs[5] = 100;
   inputs[6] = 10;
   inputs[7] = 10;
   inputs[8] = 0;
   inputIndex = 8;
   
   DDRA = 0x0F;                           //MAKE ROWS INPUT AND COLUMNS OUTPUT
   

   
   LCDcmd(0x01);         // clear display and return to the home position
   delay1ms(2);          // delay by 2 milliseconds for the clear display command to complete                              //OPEN WHILE(1)
   sendStrLCD("Serial(1)", 10);
   LCDcmd(0xC0);
   delay1ms(2);
   sendStrLCD("Board(2)", 9);
   

   
 
   while(1){
 
      do{                                 //OPEN do1
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }while(row == 0x00);                //WAIT UNTIL KEY PRESSED //CLOSE do1



      do{                                 //OPEN do2
         do{                              //OPEN do3
            delay1ms(1);                   //WAIT
            row = PORTA & 0xF0;           //READ ROWS
         }while(row == 0x00);             //CHECK FOR KEY PRESS //CLOSE do3
         
         delay1ms(15);                     //WAIT FOR DEBOUNCE
         row = PORTA & 0xF0;
      }while(row == 0x00);                //FALSE KEY PRESS //CLOSE do2

      while(1){                           //OPEN while(1)
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x01;                   //COLUMN 0 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 0
            column = 0;
            break;                        //BREAK OUT OF while(1)
         }
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x02;                   //COLUMN 1 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 1
            column = 1;
            break;                        //BREAK OUT OF while(1)
         }

         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x04;                   //COLUMN 2 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 2
            column = 2;
            break;                        //BREAK OUT OF while(1)
         }
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x08;                   //COLUMN 3 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 3
            column = 3;
            break;                        //BREAK OUT OF while(1)
         }
         row = 0;                         //KEY NOT FOUND
      break;                              //step out of while(1) loop to not get stuck
      }                                   //end while(1)

      if(row == 0x10){
         if (column == 0){
           if (inputIndex == 8){
             inputs[8] = 1;
             serialOrBoard = 1;
             LCDcmd(0x01);         // clear display and return to the home position
             delay1ms(2);
             break;
           }
           inputs[inputIndex] += 10; 
         } else if (column == 1){
           if (inputIndex == 8){
             inputIndex = 0;
             LCDcmd(0x01);         // clear display and return to the home position
             delay1ms(2); 
             continue;
           }
           inputs[inputIndex] += 1; 
         }else if (column == 2){
           inputs[inputIndex] -= 1; 
         }else if (column == 3){
           inputs[inputIndex] -= 10; 
         }
         
         /****** Input Error Checking ******/
         if (inputIndex == 0){
           if (inputs[inputIndex] > 160){
             inputs[inputIndex] = 160;
           }else if(inputs[inputIndex] < 20){
             inputs[inputIndex] = 20;
           }
         }else if (inputIndex == 1){
           if (inputs[inputIndex] < inputs[0]){
             inputs[inputIndex] = inputs[0];
           }else if(inputs[inputIndex] > 160){
             inputs[inputIndex] = 160;
           }
         }else if (inputIndex == 2){
           if (inputs[inputIndex] > 500){
             inputs[inputIndex] = 500;
           }else if(inputs[inputIndex] < 50){
             inputs[inputIndex] = 50;
           }
         }else if (inputIndex == 3){
           if (inputs[inputIndex] > 60){
             inputs[inputIndex] = 60;
           }else if(inputs[inputIndex] < -60){
             inputs[inputIndex] = -60;
           }
         }else if (inputIndex == 4){
           if (inputs[inputIndex] > 60){
             inputs[inputIndex] = 60;
           }else if(inputs[inputIndex] < inputs[3]){
             inputs[inputIndex] = inputs[3];
           }
         }else if (inputIndex == 5){
           if (inputs[inputIndex] > 500){
             inputs[inputIndex] = 500;
           }else if(inputs[inputIndex] < 50){
             inputs[inputIndex] = 50;
           }
         }else if (inputIndex == 6){
           if (inputs[inputIndex] > 30){
             inputs[inputIndex] = 30;
           }else if(inputs[inputIndex] < 1){
             inputs[inputIndex] = 1;
           }
         }else if (inputIndex == 7){
           if (inputs[inputIndex] > 10){
             inputs[inputIndex] = 10;
           }else if(inputs[inputIndex] < 1){
             inputs[inputIndex] = 1;
           }
         }
         
         LCDcmd(0x01);         // clear display and return to the home position
         delay1ms(2);          // delay by 2 milliseconds for the clear display command to complete
         sendStrLCD(menuString[inputIndex], 10);
         if (inputs[inputIndex] < 0){
           sendStrLCD("-", 2);
         }
         lenString = num2str(abs(inputs[inputIndex]), numString); 
         sendStrLCD(numString, lenString);
 
      }
      else if(row == 0x20){
         //PORTB=inputs[inputIndex];
 
      }
      else if(row == 0x40){
         //PORTB=keypad[2][column];
 
      }
      else if(row == 0x80 && inputIndex != 8){
         if (column == 0){
            inputIndex++;
         }
         if (inputIndex >= 8){
           LCDcmd(0x01);         // clear display and return to the home position
           delay1ms(2);          // delay by 2 milliseconds for the clear display command to complete
           break;
         }
         LCDcmd(0x01);         // clear display and return to the home position
         delay1ms(2);          // delay by 2 milliseconds for the clear display command to complete                              //OPEN WHILE(1)
         sendStrLCD(menuString[inputIndex], 10);
         if (inputs[inputIndex] < 0){
           sendStrLCD("-", 2);
         }
         lenString = num2str(abs(inputs[inputIndex]), numString); 
         sendStrLCD(numString, lenString);
 
      }

      do{
         delay1ms(15);
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }while(row != 0x00);                //MAKE SURE BUTTON IS NOT STILL HELD
   }

	minHead = inputs[0];
   maxHead = inputs[1];
   dMThetaHead = inputs[2];
   minTilt = inputs[3];
   maxTilt = inputs[4];
   dMThetaTilt = inputs[5];
   numSamp = inputs[6];
   freqSamp = inputs[7];
	
} 