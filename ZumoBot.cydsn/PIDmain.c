//
//
//#include <project.h>
//#include <stdio.h>
//#include "Motor.h"
//#include "Ultra.h"
//#include "Nunchuk.h"
//#include "Reflectance.h"
//#include "I2C_made.h"
//#include "Gyro.h"
//#include "Accel_magnet.h"
//#include "IR.h"
//#include "Ambient.h"
//#include "Beep.h"
//#include "Pathfinding.h"
//#include "Playsound.h"
//
//
//
//void batteryAlert(void);
//int rread(void);
//
//int main()
//{
//    
//    //Timo has said:each sensor has different sensitiveness
//    //So we should consider the reflected values respectively
//    //we tested thresholds of each sensor and then put the values into set_threshold function as parameters
//    int black_l1 = 17500;
//    int black_l3 = 21000;
//    int black_r1 = 22000;
//    int black_r3 = 21500;
//
//    int white_l1 = 4500;
//    int white_l3 = 5700;
//    int white_r1 = 4500;
//    int white_r3 = 6300;
//
//    int max_diff_l1 = black_l1 - white_l1;
//    int max_diff_l3 = black_l3 - white_l3;
//    int max_diff_r1 = black_r1 - white_r1;
//    int max_diff_r3 = black_r3 - white_r3;
//    
//    
//    float measured_value_l1,measured_value_l2,measured_value_r1,measured_value_r2;
//    float diff_left = 0;
//    float diff_right = 0;
//    
//    
//    //initialize essential components  
//    CyGlobalIntEnable; 
//    UART_1_Start();
//    
//    sensor_isr_StartEx(sensor_isr_handler);
//    reflectance_set_threshold(black_l3,black_l1,black_r1,black_r3);
//    reflectance_start();
//    ADC_Battery_Start();
//    IR_led_Write(1);
//    
//    
//    //initialize essential variables
//    struct sensors_ ref;
//    struct sensors_ dig; 
//    int flag = 0;
//    int check = 0;
//
//    
//                    //kp = 300 kd = 350 performs best until now
//    int kp = 300;   //kp = 240,kd = 350 robot performs soomthly but can not go over the tight turn
//    int kd = 350;   //kp = kd = 300 can cover the whole distance but not perform soomthly
//                    //kp = 240, kd = 300 bad perform
//    
//
//    int inTheBlack = 0;
//    uint8 max_speed = 255;
//    uint8 button_read; 
//    uint8 temp,rightSpeed,leftSpeed;
//    uint8 turn;
//    
//    
//    
//   
//    //sw1_read按下读取数值为0
//    //begin to move until you press the button on the board
//    
//    /*
//    while(flag == 0){
//        button_read = SW1_Read();
//        if(button_read == 0){
//            flag = -1;
//            CyDelay(10);
//        }
//    }
//    */
//   
//    
//    
//    uint8 signal = 0;
//    while (signal == 0) {
//         signal = get_IR();
//    }
//    
//    //turn on the motor 
//    motor_start(); 
//    motor_turn(max_speed,max_speed,0);
//   
//    for(;;)
//    {      
//        // always check for the battery alert
//        batteryAlert();
// 
//        reflectance_read(&ref);
//        reflectance_digital(&dig); 
//
//        printf("---------------------------------------------------------------------\n");
//
//        printf("ref values are : %d %d %d %d \r\n", ref.l3, ref.l1, ref.r1, ref.r3);
//
//        printf("dig values are : %d %d %d %d \r\n", dig.l3, dig.l1, dig.r1,dig.r3); 
//
//        //ref大于threshold返回0，否则在白线小于thresholds返回1
//        //when the measured values are less than threshold
//        //that means the robot is off the line and we should tune the motor      
//
//        if(dig.l1 == 1){
//            
//            while(dig.l1 == 1){
//                
//                //"check" is a optional choice, we can put if condition outside the while loop
//                if(check == 0){
//                    reflectance_read(&ref);
//                    measured_value_l1 = ref.l1;
//                    CyDelay(1);
//                    check = 1;
//                }
//                
//                //maybe redundent
//                reflectance_read(&ref);
//                reflectance_digital(&dig); 
//                measured_value_l2 = ref.l1;
//                
//                //calculate the difference between 2 values of l1 sensor
//                //ensure the value of diff_left always positive
//                diff_left = measured_value_l1 - measured_value_l2;
//                if(diff_left < 0){
//                    diff_left = 0;
//                }
//                //                //the larger measured_l2, the smaller turn that the robot should veer
//                turn = kp * ( (black_l1 - measured_value_l2) / black_l1); //+ ( kd * (diff_left / max_diff_l1));
//                turn = turn + ( kd * (diff_left / max_diff_l1));
//                //turn = (max_speed * ( (black_r1 - measured_value_r2) / black_r1));
//                //turn = turn + ( 300 * (diff_right / max_diff_r1));
//                
//                if (turn > max_speed)
//                {
//                    turn = max_speed;
//                } else if(turn < 0){
//                    turn = 0;
//                }
//                motor_turn(max_speed,max_speed-turn,0);
//                
//                measured_value_l1 = measured_value_l2;
//                
//                CyDelay(1);
//               
//            }
//            
//            //after while loop, left l1 sensor has been calibrated
//            check = 0;
//            turn = 0;
//            motor_turn(max_speed,max_speed,0);
//            CyDelay(1);
//           
//            
//        }else if(dig.r1 == 1){
//            
//            while(dig.r1 == 1){
//                if(check == 0){
//                    reflectance_read(&ref);
//                    measured_value_r1 = ref.r1;
//                    CyDelay(1);
//                    check = 1;
//                }
//                
//                reflectance_read(&ref);
//                reflectance_digital(&dig); 
//                measured_value_r2 = ref.r1;
//                
//                //calculate the difference between 2 values of l1 sensor
//                diff_right = measured_value_r1 - measured_value_r2;
//                if(diff_right < 0){
//                    diff_right = 0;
//                }
//                
//                //the larger measured_l2, the smaller turn that the robot should veer
//                turn = (kp * ( (black_r1 - measured_value_r2) / black_r1)) ;//+ ( kd * (diff_right / max_diff_r1));
//                turn = turn + ( kd * (diff_right / max_diff_r1));
//                
//                
//                if (turn > max_speed)
//                {
//                    turn = max_speed;
//                } else if(turn < 0){
//                    turn = 0;
//                }
//                      
//                motor_turn(max_speed-turn,max_speed,0);
//
//                measured_value_r1 = measured_value_r2;
//                
//                CyDelay(1);
//               
//            }
//            
//            //after while loop, left l1 sensor has been calibrated
//            check = 0;
//            turn = 0;
//            motor_turn(max_speed,max_speed,0);
//            CyDelay(1);
//            
//        }
//        
//        
//        
//        //By detecting the value of temp to determine which side robot should drives back
//        //temp 1 and 2 stand for rightSpeed and leftSpeed respectively
//        
//        
//        if(dig.l1 == 1 && dig.r1 == 1){
//            if(temp == 1){
//                rightSpeed = max_speed;
//            }else{
//                leftSpeed = max_speed;
//            }
//        }else if (dig.l1 == 1){
//            temp = 1;   
//        }else if(dig.r1 == 1){
//            temp = 2;
//        }
//        
//        motor_turn(leftSpeed,rightSpeed,0);
//        
//        //when the inner sensors above the black area,we set the variable = 1,otherwise = 0; 
//        if (inTheBlack == 0 && dig.l3 == 0 /*&& dig.l1 == 0 && dig.r1 == 0 */&& dig.r3 == 0){
//            inTheBlack = 1;
//        }
//        
//        if (inTheBlack == 1 && dig.r3 == 1 && dig.l3 == 1){
//       
//            inTheBlack = 2;
//        }
//        
//        if (inTheBlack == 2 && dig.l3 == 0 /*&& dig.l1 == 0 && dig.r1 == 0 */ && dig.r3 == 0){
//            inTheBlack = 3;
//        }        
//        if (inTheBlack == 3 && dig.r3 == 1 && dig.l3 == 1){
//            
//            inTheBlack = 4;
//        }
//        if (inTheBlack == 4 && dig.l3 == 0 /*&& dig.l1 == 0 && dig.r1 == 0 */&& dig.r3 == 0){
//            motor_stop();
//        }    
//        
//        /*
//        printf("flag values : %d\n",flag);
//        printf("derivative values : %d\n",derivative);
//        printf("KD * derivative values : %d\n",KD * derivative);
//        printf("turn speed values : %d\n",turn_speed);
//        printf("left speed values : %d\n",leftSpeed);
//        printf("right speed values : %d\n",rightSpeed);
//        printf("max speed values is : %d\n",max_speed);
//        */
//        printf("inTheBlack values is : %d\n",inTheBlack);
//
//    }
//}   
//
//
//void batteryAlert() {
//    ADC_Battery_Start();        
//    int16 adcresult =0;
//    float volts = 0.0;
//    ADC_Battery_StartConvert();
//        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   
//            adcresult = ADC_Battery_GetResult16();
//            volts = ADC_Battery_CountsTo_Volts(adcresult)*1.5;
//            printf("Volts: %.2f\n", volts);
//            if(volts<=4.0 && volts >=1.6){
//                //Beep(500,200);
//                CyDelay(2000);
//            }
//        }
//}
//
///*//battery level//
//int main()
//{
//    CyGlobalIntEnable; 
//    UART_1_Start();
//    ADC_Battery_Start();        
//    int16 adcresult =0;
//    float volts = 0.0;
//
//    printf("\nBoot\n");
//    //BatteryLed_Write(1); // Switch led on 
//    BatteryLed_Write(0); // Switch led off 
//    //uint8 button;
//    //button = SW1_Read(); // read SW1 on pSoC board
//
//    for(;;)
//    {
//        
//        ADC_Battery_StartConvert();
//        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
//            adcresult = ADC_Battery_GetResult16();
//            // Convert by multiple by 1.5 (big Timo)
//            volts = ADC_Battery_CountsTo_Volts(adcresult)*1.5;                  // convert value to Volts
//        
//            // If you want to print value
//            printf("%d %f\r\n",adcresult, volts);
//        }
//        CyDelay(500);
//        
//    }
// }   
////*/
//
//
///*//ultra sonic sensor//
//int main()
//{
//    CyGlobalIntEnable; 
//    UART_1_Start();
//    Ultra_Start();                          // Ultra Sonic Start function
//    while(1) {
//        //If you want to print out the value  
//        printf("distance = %5.0f\r\n", Ultra_GetDistance());
//        CyDelay(1000);
//    }
//}   
////*/
//
//
///*//nunchuk//
//int main()
//{
//    CyGlobalIntEnable; 
//    UART_1_Start();
//  
//    nunchuk_start();
//    nunchuk_init();
//    
//    for(;;)
//    {    
//        nunchuk_read();
//    }
//}   
////*/
//
//
///*//IR receiver//
//int main()
//{
//    CyGlobalIntEnable; 
//    UART_1_Start();
//    
//    unsigned int IR_val; 
//    
//    for(;;)
//    {
//       IR_val = get_IR();
//       printf("%x\r\n\n",IR_val);
//    }    
// }   
////*/
//
//
///*//Ambient light sensor//
//int main()
//{
//    CyGlobalIntEnable; 
//    UART_1_Start();
//    
//    I2C_Start();
//    
//    I2C_write(0x29,0x80,0x00);          // set to power down
//    I2C_write(0x29,0x80,0x03);          // set to power on
//    
//    for(;;)
//    {    
//        uint8 Data0Low,Data0High,Data1Low,Data1High;
//        Data0Low = I2C_read(0x29,CH0_L);
//        Data0High = I2C_read(0x29,CH0_H);
//        Data1Low = I2C_read(0x29,CH1_L);
//        Data1High = I2C_read(0x29,CH1_H);
//        
//        uint8 CH0, CH1;
//        CH0 = convert_raw(Data0Low,Data0High);      // combine Data0
//        CH1 = convert_raw(Data1Low,Data1High);      // combine Data1
//
//        double Ch0 = CH0;
//        double Ch1 = CH1;
//        
//        double data = 0;
//        data = getLux(Ch0,Ch1);
//        
//        // If you want to print out data
//        //printf("%lf\r\n",data);    
//    }    
// }   
////*/
//
//
///*//accelerometer//
//int main()
//{
//    CyGlobalIntEnable; 
//    UART_1_Start();
//  
//    I2C_Start();
//  
//    uint8 X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A;
//    int16 X_AXIS_A, Y_AXIS_A, Z_AXIS_A;
//    
//    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL1_REG, 0x37);           // set accelerometer & magnetometer into active mode
//    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL7_REG, 0x22);
//    
//    
//    for(;;)
//    {
//        //print out accelerometer output
//        X_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_L_A);
//        X_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_H_A);
//        X_AXIS_A = convert_raw(X_L_A, X_H_A);
//        
//        Y_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_L_A);
//        Y_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_H_A);
//        Y_AXIS_A = convert_raw(Y_L_A, Y_H_A);
//        
//        Z_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_L_A);
//        Z_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_H_A);
//        Z_AXIS_A = convert_raw(Z_L_A, Z_H_A);
//        
//        printf("ACCEL: %d %d %d %d %d %d \r\n", X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A);
//        value_convert_accel(X_AXIS_A, Y_AXIS_A, Z_AXIS_A);
//        printf("\n");
//        
//        CyDelay(50);
//    }
//}   
////*/
//
//
///*//reflectance//
//int main()
//{
//    struct sensors_ ref;
//    struct sensors_ dig;
//    CyGlobalIntEnable; 
//    UART_1_Start();
//  
//    sensor_isr_StartEx(sensor_isr_handler);
//    
//    reflectance_start();
//
//    IR_led_Write(1);
//    for(;;)
//    {
//        reflectance_read(&ref);
//        printf("%d %d %d %d \r\n", ref.l3, ref.l1, ref.r1, ref.r3);       //print out each period of reflectance sensors
//        reflectance_digital(&dig);      //print out 0 or 1 according to results of reflectance period
//        printf("%d %d %d %d \r\n", dig.l3, dig.l1, dig.r1, dig.r3);        //print out 0 or 1 according to results of reflectance period
//        
//        CyDelay(500);
//    }
//}   
////*/
//
// /* //motor//
//int main()
//{
//    CyGlobalIntEnable; 
//    UART_1_Start();
//
//    motor_start();              // motor start
//
//    motor_forward(100,2000);     // moving forward
//    motor_turn(200,50,2000);     // turn
//    motor_turn(50,200,2000);     // turn
//    motor_backward(100,2000);    // movinb backward
//       
//    motor_stop();               // motor stop
//    
//    for(;;)
//    {
//
//    }
//}
////*/
//    
//
///*//gyroscope//
//int main()
//{
//    CyGlobalIntEnable; 
//    UART_1_Start();
//  
//    I2C_Start();
//  
//    uint8 X_L_G, X_H_G, Y_L_G, Y_H_G, Z_L_G, Z_H_G;
//    int16 X_AXIS_G, Y_AXIS_G, Z_AXIS_G;
//    
//    I2C_write(GYRO_ADDR, GYRO_CTRL1_REG, 0x0F);             // set gyroscope into active mode
//    I2C_write(GYRO_ADDR, GYRO_CTRL4_REG, 0x30);             // set full scale selection to 2000dps    
//    
//    for(;;)
//    {
//        //print out gyroscope output
//        X_L_G = I2C_read(GYRO_ADDR, OUT_X_AXIS_L);
//        X_H_G = I2C_read(GYRO_ADDR, OUT_X_AXIS_H);
//        X_AXIS_G = convert_raw(X_H_G, X_L_G);
//        
//        
//        Y_L_G = I2C_read(GYRO_ADDR, OUT_Y_AXIS_L);
//        Y_H_G = I2C_read(GYRO_ADDR, OUT_Y_AXIS_H);
//        Y_AXIS_G = convert_raw(Y_H_G, Y_L_G);
//        
//        
//        Z_L_G = I2C_read(GYRO_ADDR, OUT_Z_AXIS_L);
//        Z_H_G = I2C_read(GYRO_ADDR, OUT_Z_AXIS_H);
//        Z_AXIS_G = convert_raw(Z_H_G, Z_L_G);
//     
//        // If you want to print value
//        printf("%d %d %d \r\n", X_AXIS_G, Y_AXIS_G, Z_AXIS_G);
//        CyDelay(50);
//    }
//}   
////*/
//
//
///*//magnetometer//
//int main()
//{
//    CyGlobalIntEnable; 
//    UART_1_Start();
//  
//    I2C_Start();
//   
//    uint8 X_L_M, X_H_M, Y_L_M, Y_H_M, Z_L_M, Z_H_M;
//    int16 X_AXIS, Y_AXIS, Z_AXIS;
//    
//    I2C_write(GYRO_ADDR, GYRO_CTRL1_REG, 0x0F);             // set gyroscope into active mode
//    I2C_write(GYRO_ADDR, GYRO_CTRL4_REG, 0x30);             // set full scale selection to 2000dps
//    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL1_REG, 0x37);           // set accelerometer & magnetometer into active mode
//    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL7_REG, 0x22);
//    
//    
//    for(;;)
//    {
//        X_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_X_L_M);
//        X_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_X_H_M);
//        X_AXIS = convert_raw(X_L_M, X_H_M);
//        
//        Y_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_Y_L_M);
//        Y_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_Y_H_M);
//        Y_AXIS = convert_raw(Y_L_M, Y_H_M);
//        
//        Z_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_Z_L_M);
//        Z_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_Z_H_M);
//        Z_AXIS = convert_raw(Z_L_M, Z_H_M);
//        
//        heading(X_AXIS, Y_AXIS);
//        printf("MAGNET: %d %d %d %d %d %d \r\n", X_L_M, X_H_M, Y_L_M, Y_H_M, Z_L_M, Z_H_M);
//        printf("%d %d %d \r\n", X_AXIS,Y_AXIS, Z_AXIS);
//        CyDelay(50);      
//    }
//}   
////*/
//
//
//#if 0
//int rread(void)
//{
//    SC0_SetDriveMode(PIN_DM_STRONG);
//    SC0_Write(1);
//    CyDelayUs(10);
//    SC0_SetDriveMode(PIN_DM_DIG_HIZ);
//    Timer_1_Start();
//    uint16_t start = Timer_1_ReadCounter();
//    uint16_t end = 0;
//    while(!(Timer_1_ReadStatusRegister() & Timer_1_STATUS_TC)) {
//        if(SC0_Read() == 0 && end == 0) {
//            end = Timer_1_ReadCounter();
//        }
//    }
//    Timer_1_Stop();
//    
//    return (start - end);
//}
//#endif
//
///* Don't remove the functions below */
//int _write(int file, char *ptr, int len)
//{
//    (void)file; /* Parameter is not used, suppress unused argument warning */
//	int n;
//	for(n = 0; n < len; n++) {
//        if(*ptr == '\n') UART_1_PutChar('\r');
//		UART_1_PutChar(*ptr++);
//	}
//	return len;
//}
//
//int _read (int file, char *ptr, int count)
//{
//    int chs = 0;
//    char ch;
// 
//    (void)file; /* Parameter is not used, suppress unused argument warning */
//    while(count > 0) {
//        ch = UART_1_GetChar();
//        if(ch != 0) {
//            UART_1_PutChar(ch);
//            chs++;
//            if(ch == '\r') {
//                ch = '\n';
//                UART_1_PutChar(ch);
//            }
//            *ptr++ = ch;
//            count--;
//            if(ch == '\n') break;
//        }
//    }
//    return chs;
//}
///* [] END OF FILE */
//