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
#include "Pathfinding.h"
#include "Playsound.h"

#define MOTOR_SPEED 255
#define MOTOR_SL_SPEED 140 // depend on battery
#define SENSOR_TH 10000
#define TURN_LEFT 0
#define TURN_RIGHT 1
#define TURN_SHARP_LEFT 2
#define TURN_SHARP_RIGHT 3

/**
*   Method for easier recognize the turning of the robot
*   if it a SHARP turn, then the other wheel speed is 0
*   if it a normal turn, then the other wheel speed is at SL_SPEED
* 
*/
void MyTurn(uint8 direction) {
    if (direction == TURN_RIGHT) {        
        motor_turn_adv(MOTOR_SPEED, 0, MOTOR_SL_SPEED, 0, 0);
        
    } else if (direction == TURN_LEFT) {
        motor_turn_adv(MOTOR_SL_SPEED, 0, MOTOR_SPEED, 0, 0);  
        
    } else if (direction == TURN_SHARP_RIGHT) {
        motor_turn_adv(MOTOR_SPEED, 0, 0, 0, 0); 
        
    } else if (direction == TURN_SHARP_LEFT) {
        motor_turn_adv(0, 0, MOTOR_SPEED, 0, 0);   
    }
}

    // For PD, but will not useed in basic & advance
    int r3W = 6200;
    int r1W = 4300;
    int l1W = 3700;
    int l3W = 5000;
    
    int r3B = 23900;
    int r1B = 23900;
    int l1B = 23900;
    int l3B = 23900;


//Using advance logic pathfinding
void PathfindingAdvP(uint16_t r1, uint16_t r3, uint16_t l1, uint16_t l3, uint8_t *checkStop, uint8_t *lastBlack) 
{        
    //Normalized data
    //Start normalized all measured data to [0;1]
    float Pl3 = (l3 - l3W)/(l3B - l3W);
    float Pl1 = (l1 - l1W)/(l1B - l1W);
    float Pr3 = (r3 - r3W)/(r3B - r3W);
    float Pr1 = (r1 - r1W)/(r1B - r1W);
    
    
    // 1 is black
    if(Pr1<=0.5 || Pl1<=0.5){
       
        if(l3>=SENSOR_TH){
            MyTurn(TURN_SHARP_LEFT);
            BatteryLed_Write(0);
            
        }
        else if(r3>=SENSOR_TH ){
            MyTurn(TURN_SHARP_RIGHT);
            BatteryLed_Write(0);
        }
        
        // 0100
        else if (l3 < SENSOR_TH && Pl1 >= 0.5 && Pr1 < 0.1 && r3 < SENSOR_TH){
            motor_turn(110,255,0);
            BatteryLed_Write(0);
        }
        
        else if (l3 < SENSOR_TH && Pl1 >= 0.5 && Pr1 < 0.15 && r3 < SENSOR_TH){
            motor_turn(140,255,0);
            BatteryLed_Write(0);
        }
        
        else if (l3 < SENSOR_TH && Pl1 >= 0.5 && Pr1 < 0.2 && r3 < SENSOR_TH){
            motor_turn(180,255,0);
            BatteryLed_Write(1);
        }
        
        else if (l3 < SENSOR_TH && Pl1 >= 0.5 && Pr1 < 0.3 && r3 < SENSOR_TH){
            motor_turn(255,255,0);
            BatteryLed_Write(0);
        }
        
        //0010
        else if (l3 < SENSOR_TH && Pl1 < 0.1 && Pr1 >= 0.5 && r3 < SENSOR_TH){
            motor_turn(255, 110,0);
            BatteryLed_Write(0);
        }
        else if (l3 < SENSOR_TH && Pl1 < 0.15 && Pr1 >= 0.5 && r3 < SENSOR_TH){
            motor_turn(255, 140,0);
            BatteryLed_Write(0);
        }
        else if (l3 < SENSOR_TH && Pl1 < 0.2 && Pr1 >= 0.5 && r3 < SENSOR_TH){
            motor_turn(255, 180,0);
            BatteryLed_Write(1);
        }
        else if (l3 < SENSOR_TH && Pl1 < 0.3 && Pr1 >= 0.5 && r3 < SENSOR_TH){
            motor_turn(255, 255,0);
            BatteryLed_Write(0);
        }
    } 
    
    else {
        motor_forward(MOTOR_SPEED,0);
        BatteryLed_Write(0);
    }
    
    
    if (*lastBlack == 1 && (l3 <SENSOR_TH || r3 < SENSOR_TH)) {
        *checkStop = *checkStop + 1;        
        *lastBlack = 0;
    }
    
    // STOP AT FINISH LINE
    if(l3>=SENSOR_TH && l1>=SENSOR_TH && r1>=SENSOR_TH && r3>=SENSOR_TH) {
        *lastBlack = 1;
    }    
    
    if (*checkStop>=2) {
        *checkStop = 2;
        motor_stop();
        CyDelay(3000);
        //Playsound();
    }             
}

//Using advance logic pathfinding
void PathfindingAdv(uint16_t r1, uint16_t r3, uint16_t l1, uint16_t l3, uint8_t *checkStop, uint8_t *lastBlack) 
{        
    // 1 is black
    if(r1<=SENSOR_TH || l1<=SENSOR_TH){
       
        if(l3>=SENSOR_TH) MyTurn(TURN_SHARP_LEFT);
        
        else if(r3>=SENSOR_TH ) MyTurn(TURN_SHARP_RIGHT);
        
        // 0100
        else if (l3 < SENSOR_TH && l1 >= SENSOR_TH && r1 < SENSOR_TH && r3 < SENSOR_TH)
            MyTurn(TURN_LEFT);
        
         //0010
        else if (l3 < SENSOR_TH && l1 < SENSOR_TH && r1 >= SENSOR_TH && r3 < SENSOR_TH)
            MyTurn(TURN_RIGHT);        
    } 
    
    else {
        motor_forward(MOTOR_SPEED,0);    
    }
    
    
    if (*lastBlack == 1 && (l3 <SENSOR_TH || r3 < SENSOR_TH)) {
        *checkStop = *checkStop + 1;
        BatteryLed_Write(1);
        *lastBlack = 0;
    }
    
    // STOP AT FINISH LINE
    if(l3>=SENSOR_TH && l1>=SENSOR_TH && r1>=SENSOR_TH && r3>=SENSOR_TH) {
        *lastBlack = 1;
    }    
    
    if (*checkStop>=2) {
        *checkStop = 2;
        motor_stop();
        CyDelay(3000);
        //Playsound();
    }             
}

//Using Manh basic logic pathfinding
void PathfindingBasic(uint16_t r1, uint16_t r3, uint16_t l1, uint16_t l3, uint8_t *checkStop, uint8_t *lastBlack) 
{        
    if(r1<=SENSOR_TH || l1<=SENSOR_TH){
       
        if(l3>=SENSOR_TH){
            motor_turn(0,MOTOR_SPEED,0);            
        }
        else if(r3>=SENSOR_TH ){
            motor_turn(MOTOR_SPEED,0,0);
        }
       
    } else {
         motor_forward(MOTOR_SPEED, 0);    
    }
    
    
    if (*lastBlack == 1 && (l3 <SENSOR_TH || r3 < SENSOR_TH)) {
        *checkStop = *checkStop + 1;
        BatteryLed_Write(1);
        *lastBlack = 0;
    }
    
    // STOP AT FINISH LINE
    if(l3>=SENSOR_TH && l1>=SENSOR_TH && r1>=SENSOR_TH && r3>=SENSOR_TH) {
        *lastBlack = 1;
    }    
    
    if (*checkStop>=2) {
        *checkStop = 2;
        motor_stop();
        CyDelay(3000);
        //Playsound();
    }             
}

/*    
    if(r1<=SENSOR_TH || l1<=SENSOR_TH){
       
        if(l3>=SENSOR_TH){
            motor_turn(0,TURNSPEED,0);
            
        }
        else if(r3>=SENSOR_TH ){
            motor_turn(TURNSPEED,0,0);
        }
       
    } else {
         motor_forward(MOTOR_SPEED, 0);    
    }
    

    // Simplified
    if(l3>=SENSOR_TH){
        motor_turn(0,TURNSPEED,0);
    } else if(r3>=SENSOR_TH){
        motor_turn(TURNSPEED,0,0);
    } else {
         motor_forward(MOTOR_SPEED, 0);    
    }

    // STOP AT FINISH LINE
    if(l3>=SENSOR_TH && l1>=SENSOR_TH && r1>=SENSOR_TH && r3>=SENSOR_TH) {
        motor_forward(MOTOR_SPEED,0);  
        *lastBlack = 1;
    }
    
   
    if (*lastBlack == 1 && (l3 < SENSOR_TH || r3 < SENSOR_TH)) {
        *checkStop = *checkStop + 1;
        *lastBlack = 0;
    }
    
    if (*checkStop>=2) {
        *checkStop = 2;
        motor_stop();
        CyDelay(3000);
        //Playsound();
    }             

//Advance

    // 1 is black
    // 0100
    if (l3 < SENSOR_TH && l1 >= SENSOR_TH && r1 < SENSOR_TH && r3 < SENSOR_TH) {
        MyTurn(TURN_LEFT);
    }
    
    // 0010
    else if (l3 < SENSOR_TH && l1 < SENSOR_TH && r1 >= SENSOR_TH && r3 < SENSOR_TH) {
        MyTurn(TURN_RIGHT);
    }
    
    //1100
    else if (l3 >= SENSOR_TH && l1 >= SENSOR_TH && r1 < SENSOR_TH && r3 < SENSOR_TH ) {
        MyTurn(TURN_SHARP_LEFT);
    }
    
    //1000
    else if (l3 >= SENSOR_TH && l1 < SENSOR_TH && r1 < SENSOR_TH && r3 < SENSOR_TH ) {
        MyTurn(TURN_SHARP_LEFT); 
    }
    
    //0001
    else if (l3 < SENSOR_TH && l1 < SENSOR_TH && r1 < SENSOR_TH && r3 >= SENSOR_TH ) {
        MyTurn(TURN_SHARP_RIGHT);   
    }
    
    //0011
    else if (l3 < SENSOR_TH && l1 < SENSOR_TH && r1 >= SENSOR_TH && r3 >= SENSOR_TH ) {
        MyTurn(TURN_SHARP_RIGHT);  
    }
    
    //0000
    else if (l3 < SENSOR_TH && l1 < SENSOR_TH && r1 < SENSOR_TH && r3 < SENSOR_TH ) {
    }
    
    //1001
    else if (l3 >= SENSOR_TH && l1 < SENSOR_TH && r1 < SENSOR_TH && r3 >= SENSOR_TH ) {
    }    
        
    else {
        motor_forward(MOTOR_SPEED,0);    
    }
    */
    // PID IF ELSE
    /*

// Convert to 0 or 1
        // Black line = 1
        // White line = 0
        if (ref.l3 >= SENSOR_TH) {
            l3 = 1;
        } else {
            l3 = 0;    
        }
        
        if (ref.l1 >= SENSOR_TH) {
            l1 = 1;
        } else {
            l1 = 0;    
        }
        
        if (ref.r1 >= SENSOR_TH) {
            r1 = 1;
        } else {
            r1 = 0;    
        }
        
        if (ref.r3 >= SENSOR_TH) {
            r3 = 1;
        } else {
            r3 = 0;    
        }
        
        printf("%d %d %d %d \r\n", l3, l1, r1, r3); 
        
        error = l3*-2 + l1*-1 + r1*1 + r3*2;
        
        fsum = l3 + l1 + r1 + r3;
        if (fsum > 0) {
            BatteryLed_Write(0);
            error = error/fsum;    
        } else {
        //1001
        //0000
        //0110
        //1111
            //0000
            if (l3==0&&l1==0&&r1==0&&r3==0) {
                error = 2*last_error;
                BatteryLed_Write(0);
            }
            
            //1001
            else if (l3==1&&l1==0&&r1==0&&r3==1) {
                error = last_error;
                BatteryLed_Write(0);
            }
            //0110
            //1111
            else {
                motor_forward(255,0);
                BatteryLed_Write(1);
            }
        }
        
        //intergral = intergral + error;
        
        derivaties = error - last_error;
        
        last_error = error;
        output = (Kp*error + Kd*derivaties);//PD
        
        
        LeftSpeed = 255 + output; 
        RightSpeed = 255 - output; 
        
        if (LeftSpeed > 255) LeftSpeed = 255;
        if (LeftSpeed < 0) LeftSpeed = 0;
        
        if (RightSpeed > 255) RightSpeed = 255;
        if (RightSpeed < 0) RightSpeed = 0;
        
        printf("error %f, fsum %d, intergral %f, derivaties %f, output %f, turn %f", error, fsum, intergral, derivaties, output,turn);
        printf("Leftspeed = %f. Rightspeed = %f\n", LeftSpeed, RightSpeed);
        
        motor_turn(LeftSpeed, RightSpeed,0);

//*******************************************PD
    // Higher value make it more wobblier, however it make follow the line better
    // Basically just a more fancy ways to say if else statement
    // If 0010 then move a little = 1 * Kp
    // If 0001 then move a lot = 3 * Kp
    // The adjustment factor is Kp
    // use (speed + Kp) to turn, the other wheel will slow to (speed - Kp).
    // If Kp >=  speed/3, Ki Kd = 0, and we at 0001 or 1000 mean we turned to Manh method.   
    // Kp high ==> robot whooblier but turn better
    // Kp low ==> less whooblier, turn worse
    float Kp = 100;
    
    // Error leverage.
    // More error = positive => high output value = faster turn
    // Less error = negative => low output value = lesser turn 
    // Make smoother the turnning of robot without pumping Kp like a steroid user.
    // So if the robot managed to go 0010 without change error it can still go like that.
    // Charming if you have around 10 sensors
    // With 4 sensors it just stupid
    // 4 sensors, error can only have 4 value:
    // -4, -1, 1, 4. One change in error = huge jump in output value
    // For example from 0010 to 0001, speed change = 3*Kd
    // If Kd >= 50, then it just a long and stupid way to use Manh's code
    // Kd high ==> robot whooblier but turn better
    // Kd low ==> less whooblier, turn worse
    float Kd = 23;
    
    // The most valuable constant in this PID method
    // This is what make it different from logic method
    // But it does not mean it's better
    // Use it as it is will make the code stupider.
    // It's the cumulative amount of error
    // Which mean if we try run the track several times
    // The error will cumulative. The value will not reset
    // so after a several time running Ki will have big effect
    // Since is cumulative, after several run time, if it does not increase
    // which mean you're tunning the robot successfully.
    // But running without error also mean slower speed.
    // So it a trade-off.
    // So it useless.
    // So we don't use it
    // float Ki = 1;
    
    float error = 0;
    float last_error = 0;
    float LeftSpeed, RightSpeed = 0;
    float intergral, derivaties, output = 0;
    int r3W = 6200;
    int r1W = 4300;
    int l1W = 3700;
    int l3W = 5000;
    
    int r3B = 23900;
    int r1B = 23900;
    int l1B = 23900;
    int l3B = 23900;
    float l3,l1,r1,r3, fsum;

    //******************** inside the loop
    
        //Start normalized all measured data to [0;1]
        l3 = (ref.l3 - l3W)/(l3B - l3W);
        l1 = (ref.l1 - l1W)/(l1B - l1W);
        r3 = (ref.r3 - r3W)/(r3B - r3W);
        r1 = (ref.r1 - r1W)/(r1B - r1W);
        
        error = l3*-2 + l1*-1 + r1*1 + r3*2;
        
        // maintain the turn even if out of white line
        if (l3<0.5&&l1<0.5&&r1<0.5&&r3<0.5) {
            error = last_error;    
        }
        //intergral = intergral + error;
        derivaties = error - last_error;
        last_error = error;
        output = (Kp*error + Kd*derivaties);//PD
        
        LeftSpeed = 190 + output; 
        RightSpeed = 190 - output; 
        
        if (LeftSpeed > 255) LeftSpeed = 255;
        if (LeftSpeed < 0) LeftSpeed = 0;
        
        if (RightSpeed > 255) RightSpeed = 255;
        if (RightSpeed < 0) RightSpeed = 0;
        
        printf("error %f, intergral %f, derivaties %f, output %f", error, intergral, derivaties, output);
        printf("Leftspeed = %f. Rightspeed = %f\n", LeftSpeed, RightSpeed);
        
        motor_turn(LeftSpeed, RightSpeed,0);
*/

