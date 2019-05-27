/**
* @file sensors.h
* @author: Group 1 Wednesday
* @date June 2018
* @brief Header file macros and functions used for gathering sensor data
*/

void setAlarm1(uint16_t msDelay1);
void delay1(uint16_t msDelay1);
void Init_TC6_TC2 (void);

#define laser_wr  0xc4
#define laser_rd  0xc5

#define gyro_wr 0xD2
#define gyro_rd 0xD3

#define accel_wr 0xA6    //
#define accel_rd 0xA7    // 
#define ADXL345_TO_READ 6
 
#define ADXL345_POWER_CTL 0x2D
#define ADXL345_DATAX0 0x32
#define ADXL345_DATA_FORMAT 0x31
 
#define ADXL345_OFSX 0x1E
#define ADXL345_OFSY 0x1F
#define ADXL345_OFSZ 0x20
 
#define ALPHA 0.5

#define magnet_wr  0x3C
#define magnet_rd  0x3D

#define HM5883_MODE_REG 0x02
#define HM5883_DATAX0 0x03


#define PI 3.14159
#define BUFF_SIZE	100
#define ANG_SIZE 4


        

void adxl345_getrawdata(int *axraw, int *ayraw, int *azraw);
void accel_init(void);
void accel_test(void);
void accel_calc(int *axraw, int *ayraw, int *azraw);

void hm5883_getrawdata(int *mxraw, int *myraw, int *mzraw);
void magnet_init(void);
void magnet_test(void);
void magnet_calc(int *myraw, int *mzraw);


void l3g4200d_getrawdata(int *gxraw, int *gyraw, int *gzraw);
void gyro_init(void);
void gyro_test(void);
void gyro_calc(int *gxraw, int *gyraw, int *gzraw);
void gyro_str(int gpos, char *str);


      