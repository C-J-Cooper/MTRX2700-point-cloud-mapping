/**
* @file functions.h
* @author: Group 1 Wednesday
* @date June 2018
* @brief Header file for functions used in the TBMS mapping system
*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H


//void sendStrLCD(char *string, int stringLen);
void LCDcmd(char cmd);
void delay1ms(unsigned dt);
void delay2us(unsigned dt);
void mainSetup(void);
int num2str(int timeHigh, char * distance);
void servoControl(long int heading, long int tilt); 
unsigned short distanceLIDAR(void);
//void mappingSystem(long int minHead, long int maxHead, long int minTilt, long int maxTilt, long int dMThetaTilt, long int dMThetaHead, int numSamp);
void sensors(double* sensorValues);
void dispOrientLCD(void);
void getUserInput(void);
void inputConfigBoard(void);

#endif