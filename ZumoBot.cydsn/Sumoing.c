/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */
#include "Reflectance.h"
#include <project.h>
#include <stdio.h>
#include "Motor.h"
#include "Sumoing.h"
#include "Ultra.h"

#define MOTOR_SPEED 255
#define MOTOR_BW_SPEED 70
#define TURNSPEED 70

#define SENSOR_TH_1 6000
#define SENSOR_TH 13000
#define TURNLEFT 0
#define TURNRIGHT 1

#define Distance 30

void MyTurnSumo(uint8 direction) {
    if (direction == TURNRIGHT) {        
        motor_turn_adv(TURNSPEED, 0, MOTOR_BW_SPEED, 1, 0);    
    } else {
        motor_turn_adv(MOTOR_BW_SPEED, 1, TURNSPEED, 0, 0);    
    }
}

// Sumo method without Ultrasonic sensor
void Sumoing(uint16_t r1, uint16_t r3, uint16_t l1, uint16_t l3)
{   
    //1111
    //move forward
    if (r1 >= SENSOR_TH_1 && r3 >= SENSOR_TH_1 && l1 >= SENSOR_TH_1 && l3 >= SENSOR_TH_1){
        motor_forward(MOTOR_SPEED, 0);
    }
    else if(r1>=SENSOR_TH || r3>=SENSOR_TH){
        motor_backward(MOTOR_SPEED,10);
        motor_turn(MOTOR_SPEED, 0, 400);    
    } 
    else if (l1>=SENSOR_TH || l3>=SENSOR_TH){        
        motor_backward(MOTOR_SPEED,10);
        motor_turn(0, MOTOR_SPEED, 400);    
        
    } 
    else {
        motor_forward(MOTOR_SPEED, 0);    
    }
}

/*

void Sumoing(uint8_t detect, uint8_t *countdown)
{
    printf("distance = %5.0f\r\n", Ultra_GetDistance());
    if (Ultra_GetDistance()< Distance || *countdown > 2 ) {        
        motor_forward(MOTOR_SPEED,0);
        if (Ultra_GetDistance()< Distance)
        {
            *countdown = 40;    
        }
    } else  {
        MyTurnSumo(TURNLEFT);    
    }
    
    if (*countdown > 2) {
        *countdown = *countdown -1;
    }
    
    printf("countdown = %d\r\n", *countdown);
    CyDelay(1);
}
*/
