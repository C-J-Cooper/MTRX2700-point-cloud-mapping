/**
* @file servoControl.c
* @author: Wednesday Group 1
* @date June, 2018
* @brief Sets the position of the servo motors.
*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "functions.h"

int degree2DutyculeLOW(long int);
int degree2DutyculeHIGH(long int);

int dutycycle;

/**
 * \fn void servoControl(long int heading, long int tilt)
 * \breif Sets the position of the servo motors.
 * \param long int heading The azimuth to set the vertial servo to.
 * \param long int tilt The angle of elevation to set the horizontal servo to. 
 */
void servoControl(long int heading, long int tilt){
  
  PWMDTY4 = degree2DutyculeHIGH(heading);
  PWMDTY5 = degree2DutyculeLOW(heading);
  
  PWMDTY6 = degree2DutyculeHIGH(tilt);
  PWMDTY7 = degree2DutyculeLOW(tilt);
  
}

/* converts input degrees to PWM duty cucle -------------------------*/
/**
 * \fn int degree2DutyculeHIGH(long int degree)
 * \breif Converts input degrees to the high section of a duty cycle
 * \param long int degree input angle in degrees
 */
int degree2DutyculeHIGH(long int degree){

  dutycycle = 4500-(90000-degree)/32;
  dutycycle = (dutycycle&0xFF00);
  dutycycle = (dutycycle>>8);
  return (dutycycle);  
}

/**
 * \fn int degree2DutyculeLOW(long int degree)
 * \breif Converts input degrees to the low section of a duty cycle
 * \param long int degree input angle in degrees
 */
int degree2DutyculeLOW(long int degree){

  dutycycle = 4500-(90000-degree)/32;
  return (dutycycle&0x00FF);  
}