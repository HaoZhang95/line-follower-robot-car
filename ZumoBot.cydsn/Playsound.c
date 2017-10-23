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
#include <project.h>
#include "Playsound.h"
#include "Beep.h"
#include "Motor.h"

// Cuong's notes
/*
#define Re7 84
#define Dom7 87
#define Do7 94
#define Si6 100
#define La6 110
#define Sol6 125
#define Fa6 135
#define Mi6 148
#define Re6 168
#define Do6 183
#define Si5 200
#define La5 223
#define Sol5 250
*/

#define D6 84
#define Cm6 87
#define C6 94
#define B7 100
#define A7 110
#define G7 125
#define F7 135
#define E7 148
#define D7 168
#define C7 183
#define B8 200
#define A8 223
#define G8 250
#define SPEED 200

/*
#define D7 255
#define Dm7 250
#define E7 230
#define F7 215
#define Fm7 205
#define G7 190
#define Gm7 175
#define A7 160
#define B7 145
#define C6 130
#define Cm6 120
#define D6 115
*/

void Dancing(uint32 length, uint8 pitch, uint8_t *i) {
    Beep(length, pitch);
    if (*i == 0) {
        BatteryLed_Write(1);
        IR_led_Write(1);
        *i = 1;
        motor_forward(SPEED, length/2);
    } else {
        BatteryLed_Write(0);
        IR_led_Write(0);
        *i = 0;
        motor_backward(SPEED, length/2);
    }
    
}

void Playsound(){
    uint8_t i = 0;
    motor_start();
    Dancing(120, D6, &i);
    CyDelay(30);
    Dancing(120, D6, &i);
    CyDelay(30);
    Dancing(120, D6, &i);
    CyDelay(30);
    Dancing(150, D6, &i);
    
    
    // F#7 + B7
    Dancing(300, 175, &i);     
    
  
    // E7 + C6
    Dancing(300, 155, &i); 
    CyDelay(10);
    
    
    // F7 + D6
    Dancing(150, 135, &i); 
     
    
    // E7 + C6
    Dancing(150, 155, &i); 
    
    
    // F7 + D6
    Dancing(600, 135, &i); 
    CyDelay(50);  
    
       
    // A G A G C6
    Dancing(300, A7, &i);
    Dancing(300, G7, &i); 
    Dancing(300, A7, &i);
    Dancing(150, G7, &i); 
    Dancing(150, C6, &i);
    
        
    Dancing(150, Cm6, &i);
    Dancing(150, C6, &i);
    Dancing(300, B7, &i);
    Dancing(150, Cm6, &i);
    Dancing(300, B7, &i);
    Dancing(150, B7, &i);
    
    
    Dancing(300, A7, &i);
    Dancing(300, G7, &i);
    Dancing(300, F7, &i);
    Dancing(150, G7, &i);
    Dancing(150, E7, &i);
    Beep(1200, E7);
    motor_turn(SPEED,0,600);
    
    
    
    CyDelay(250);
    
    // A G A G C6
    Dancing(300, A7, &i);
    Dancing(300, G7, &i); 
    Dancing(300, A7, &i);
    Dancing(150, G7, &i); 
    Dancing(150, C6, &i);
    
    
    Dancing(150, Cm6, &i);
    Dancing(150, C6, &i);
    Dancing(300, B7, &i);
    Dancing(150, Cm6, &i);
    Dancing(450, B7, &i);
    
    
    
    Dancing(300, A7, &i);
    Dancing(300, G7, &i);
    Dancing(300, A7, &i);
    Dancing(150, C6, &i);
    Dancing(150, D6, &i);    
 
    
    Beep(1200, D6);
    motor_turn(0, SPEED, 600);
}


/* [] END OF FILE */
