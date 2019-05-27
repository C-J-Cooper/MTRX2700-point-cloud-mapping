// Sensors function returning evelation, azimuth, gyro x, gyro y, gyro z. 
// Min Ho Lee
//   

 #include <hidef.h>      /* common defines and macros */
 #include <math.h>
 #include <stdlib.h>
 #include "derivative.h"      /* derivative-specific definitions */
 #include "iic.h"  
 #include "pll.h"
 #include "sci1.h"
 #include "functions.h" 
 #include "sensors.h"

 #include "l3g4200.h"  // register's definitions    ( not used by ed )
 
 volatile uint8_t alarmSignaled1 = 0; /* Flag set when alarm 1 signaled */

 volatile uint16_t currentTime1 = 0; /* variables private to timeout routines */
 uint16_t alarmTime1 = 0;
 volatile uint8_t alarmSet1 = 0;

  
 char buff[BUFF_SIZE];
 int gxraw[BUFF_SIZE];
 int gyraw[BUFF_SIZE],gzraw[BUFF_SIZE];
 double mag_azi;	
 
 int axraw[BUFF_SIZE];
 int ayraw[BUFF_SIZE],azraw[BUFF_SIZE];
 double azang, neg_tilt;	

 int mxraw[BUFF_SIZE];
 int myraw[BUFF_SIZE],mzraw[BUFF_SIZE];
 double gxpos, gypos, gzpos;
 char gxstr[ANG_SIZE], gystr[ANG_SIZE], gzstr[ANG_SIZE];	



 
 int k;



 uint16_t Dist;
 
 void sensors(double *sensorValues) {
   
 uint8_t D2W, D3R, WHO, who;
 int  res1, res2,  res3, *p, integer = 0;
 float acc;
 
 gxpos = *(sensorValues+2);
 gypos = *(sensorValues+3);        
 azang = *sensorValues;
 neg_tilt = 1;
 mag_azi = *(sensorValues+1);

 // The next 4 lines just to make sure all is working
 // are not needed for final program

 DDRB= 0xFF;   /* Port B output */
 DDRJ= 0xFF;   // Port J to Output
 PTJ = 0x00;   // enable LEDs
 PORTB=0x55;     // debuging info
 
 
 
 PLL_Init();  // make sure we are runnign at 24 Mhz
// mainSetup();
 
EnableInterrupts;

//This program will send the gyro, accelerometer adn magnetometer data
// to a serial port
// You can connect the serial port, set it to 9600 bauds 

 SCI1_Init(BAUD_9600);   // capped at 9600, if PLL inactive (4 MHz bus)
 
 Init_TC6_TC2();

 iicinit();
 
  gyro_init();     // l3g4200 setup
  accel_init();
  magnet_init();
  TC2 = TCNT + 50;
 
  while(1) {
  
    delay1(100); 

    hm5883_getrawdata(&mxraw, &myraw, &mzraw);
    magnet_calc(&myraw, &mzraw);
    adxl345_getrawdata( &axraw, &ayraw, &azraw) ;        // read data
    accel_calc(&axraw, &ayraw, &azraw);
    *sensorValues = azang;
    *(sensorValues+1) = mag_azi;
    *(sensorValues+2) = gxpos;
    *(sensorValues+3) = gypos;
    
    //sensorValues[0] = 
    
    return;
          
  }
}

  
//   ******************  END Main   *****************


// Magnetometer

void magnet_init(void){
  
  int  res1; 
  res1=iicstart(magnet_wr);
  res1=iictransmit(HM5883_MODE_REG );  // 
  res1=iictransmit(0x00 );
  iicstop(); 
 
}


void hm5883_getrawdata(int *mxraw, int *myraw, int *mzraw){
  
 uint8_t i = 0;
 uint8_t buff[6];
 int res1;
	
 res1=iicstart(magnet_wr);
 res1=iictransmit(HM5883_DATAX0 );
 res1= iicrestart(magnet_rd); 
 iicswrcv();
 
 for(i=0; i<4  ;i++) {
  buff[i]=iicreceive();
 }
 
 buff[i]= iicreceivem1();
 buff[i+1]= iicreceivelast();

	*mxraw = ((buff[0] << 8) | buff[1]);
	*myraw = ((buff[2] << 8) | buff[3]);
	*mzraw = ((buff[4] << 8) | buff[5]);
} 
 

void magnet_calc(int *myraw, int *mzraw) {
  double to_deg = 180/PI;
  mag_azi = 0;
  mag_azi = atan2(myraw[0],mzraw[0])*to_deg;
  
  if(mzraw[0]==0) {
    if(myraw[0]>0) {
      mag_azi = 0;
    }
    else if(myraw[0]<0){
      mag_azi = 270;
    } 
    else {
      mag_azi = 0;
    }
  }
  else if(mzraw[0]>0) {
  }
  else {
      mag_azi = 180 + mag_azi;
  }
}

  
//Accelerometer

void accel_init (void){
  
 int  res1;
 
 res1=iicstart(accel_wr);
 res1=iictransmit(ADXL345_POWER_CTL );  //  
 res1=iictransmit(0x08 );
  
 res1=iictransmit(ADXL345_DATA_FORMAT );  // ; 
 res1=iictransmit(0x08 );
  
 iicstop();  
}


void adxl345_getrawdata(int *axraw, int *ayraw, int *azraw){
  
 uint8_t i = 0;
 uint8_t buff[6];
 int res1;
	
 res1=iicstart(accel_wr);
 res1=iictransmit(ADXL345_DATAX0 );
 res1= iicrestart(accel_rd); 
 iicswrcv();
 
 for(i=0; i<4  ;i++) {
  buff[i]=iicreceive();
 }
 
 buff[i]= iicreceivem1();
 buff[i+1]= iicreceivelast();

	*axraw = ((buff[1] << 8) | buff[0]);
	*ayraw = ((buff[3] << 8) | buff[2]);
	*azraw = ((buff[5] << 8) | buff[4]);
}

void accel_calc(int *azraw) {
  double scale = 0.0039, gval = 9.81, to_deg = 180/PI;
  
  azang = 0, neg_tilt = 1;
  
  //if (azraw[0] < 0){
  //    neg_tilt = -1;
  //}
  azang = asin(neg_tilt*azraw[0]*scale)*to_deg;
}

  

// test the precense of Gyro , should get 211 on return 
// 

void gyro_test(void) {
 int res1,who; 
 
 res1=iicstart(0xD2);
 res1=iictransmit(L3G4200D_WHO_AM_I);
  
 res1=iicrestart(0xD3);
 who=iicreceiveone();
 //who=who & 0x00ff;     Debugging  info
 //PORTB=  who ;

}


 //  Gyro Initialisation
 
 void gyro_init (void) {
  
 int  res1;
 
 res1=iicstart(gyro_wr);
 res1=iictransmit(L3G4200D_CTRL_REG1 );  // ; 100hz, 12.5Hz, Power up
 res1=iictransmit(0x0f );
 iicstop();  
 }
 
 
// Function to get a set of gyro data
// Eduardo Nebot,30 July 2015 

void l3g4200d_getrawdata(int *gxraw, int *gyraw, int *gzraw) {
 	uint8_t i = 0;
	uint8_t buff[6];
	int res1;
	
   res1=iicstart(gyro_wr);
   res1=iictransmit(L3G4200D_OUT_XYZ_CONT );
   res1= iicrestart(gyro_rd); 
 
 iicswrcv();
 
 for(i=0; i<4  ;i++) {
  buff[i]=iicreceive();
 }
 
buff[i]= iicreceivem1();
buff[i+1]= iicreceivelast();

	*gxraw = ((buff[1] << 8) | buff[0]);
	*gyraw = ((buff[3] << 8) | buff[2]);
	*gzraw = ((buff[5] << 8) | buff[4]);
}

void gyro_calc(int *gxraw, int *gyraw, int *gzraw) {
  double sensitivity = 0.00875, sampling = 0.01;
  
  gxpos += (gxraw[0])*sensitivity*sampling*12;
  gypos += (gyraw[0])*sensitivity*sampling*12;
  gzpos += (gzraw[0])*sensitivity*sampling*12;
  
}

// ********************

void setAlarm1(uint16_t msDelay1)
{
    alarmTime1 = currentTime1 + (uint16_t)(msDelay1/16);
    alarmSet1 = 1;
    alarmSignaled1 = 0;
}


void delay1(uint16_t msec)
{
    TC6 = TCNT + 1500; // Set initial time
    setAlarm1(msec);
    while(!alarmSignaled1) {};
}



/*  Interrupt   EMN */

// interrupt(((0x10000-Vtimch7)/2)-1) void TC7_ISR(void){
// the line above is to make it portable between differen
// Freescale processors
// The symbols for each interrupt ( in this case Vtimch7 )'
// are defined in the provided variable definition file
// I am usign a much simpler definition ( vector number) 
// that is easier to understand

interrupt 14 void TC6_ISR(void) {
   
  TC6 =TCNT + 1500;   // interrupt every msec
//  TFLG1=TFLG1 | TFLG1_C6F_MASK;
  currentTime1++;
    if (alarmSet1 && currentTime1 == alarmTime1)
    {
        alarmSignaled1 = 1;
        alarmSet1 = 0;
    }
  // PORTB=0x89;        // count   (debugging)
}


// TC2 Interrupt
interrupt 10 void TC2_ISR(void) {
  
 // PORTB = 0x25;
  l3g4200d_getrawdata( &gxraw, &gyraw, &gzraw);
  gyro_calc(&gxraw, &gyraw, &gzraw); 
  TC2 =TC2 + 15000;   // interrupt every msec
}


// TC2 and TC6 Initialisation
void Init_TC6_TC2 (void) {
  
_asm SEI;
                                                        
TSCR1=0x90;
TSCR2=0x04;   // prescaler 1
TIOS=TIOS | TIOS_IOS6_MASK;
TIOS=TIOS | TIOS_IOS2_MASK;
TCTL1=0x40;
TIE=TIE | 0x40;
TIE=TIE | 0x04;
 _asm CLI;
 
}

