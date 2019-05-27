#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "functions.h"

unsigned sevenSegInx;
unsigned distGlobal;
unsigned distGlobalTemp;

void mappingSystem(long int minHead, long int maxHead, long int minTilt, long int maxTilt, long int dMThetaTilt, long int dMThetaHead, int numSamp){
  
  long int currentHead;
  long int currentTilt;
  int i;    // temp. used for delays
  int currentSampleNumber;
  int stringLength;
  char string2Print;
  double sensorValues[] = {0,0,0,0};
    
  currentSampleNumber = 0;
  currentHead = minHead;
  
  // Increase Heading
  while(currentHead <= maxHead){
     currentTilt = minTilt;
     
     // Rotate lidar up
     while(currentTilt < maxTilt){
        
        servoControl(currentTilt, currentHead);
        // feedback, check if pos is accuirite
        sensors(sensorValues);

        SCI1_OutString("Elevation:");
        if(sensorValues[0]<0) {
          SCI1_OutString("-");
        }
        SCI1_OutUDec((unsigned short) abs(sensorValues[0]));

        SCI1_OutString("Azimuth:");
        //if(sensorValues[1]<0) {
        //  SCI1_OutString("-");
        //}
       
        SCI1_OutUDec((unsigned short) abs(sensorValues[1]));

        
        SCI1_OutString("Gx:");
        if(sensorValues[2]<0) {
          SCI1_OutString("-");
        }
        SCI1_OutUDec((unsigned short) abs(sensorValues[2]));
        SCI1_OutString("Gy:");
        if(sensorValues[3]<0) {
          SCI1_OutString("-");
        }
       
        SCI1_OutUDec((unsigned short) abs(sensorValues[3]));
        
                
        SCI1_OutString("\n");

        // Measure distance sample number times
        while (currentSampleNumber < numSamp){
          
          distGlobal = distanceLIDAR();           // get the distance from the LIDAR
          sevenSegInx = 4;
          TC5 = TCNT + 30; 
          // send distance sensor reading to SERIAL
          currentSampleNumber++;
        }
        currentSampleNumber = 0;
        i = 1;
        while(i>0){
          
          delay1ms(50);          // delay by 2 milliseconds for the clear display command to complete
          --i;
        }
        currentTilt += dMThetaTilt;
     }
     currentHead += dMThetaHead;
     
     // Rotate lidar down
     while(currentTilt >= minTilt){
      
       servoControl(currentTilt, currentHead);
        // feedback, check if pos it accurite 
        
        // Measure distance sample number times
        while (currentSampleNumber < numSamp){
          
          distGlobal = distanceLIDAR();           // get the distance from the LIDAR
          sevenSegInx = 4;
          TC5 = TCNT + 30; 
          // send distance to SERIAL
          currentSampleNumber++;
        }
        currentSampleNumber = 0;
       i = 1;
        while(i>0){
          
          delay1ms(50);          // delay by 2 milliseconds for the clear display command to complete
          --i;
        }
       currentTilt -= dMThetaTilt;
      
     }
     currentHead += dMThetaHead;      
  }
  
}

// Interrupt for 7-Seg displace for distance
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
/*
// TC2 Interrupt
interrupt 10 void TC2_ISR(void) {
  
 // PORTB = 0x25;
  l3g4200d_getrawdata( &gxraw, &gyraw, &gzraw);
  gyro_calc(&gxraw, &gyraw, &gzraw); 
  TC2 =TC2 + 15000;   // interrupt every msec
}

// TC2 Initialisation
void Init_TC2 (void) {
  
_asm SEI;
                                                        
TSCR1=0x90;
TSCR2=0x04;   // prescaler 1
TIOS=TIOS | TIOS_IOS2_MASK;
TCTL1=0x40;
TIE=TIE | 0x04;
 _asm CLI;
 
}*/