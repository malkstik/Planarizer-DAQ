/** @file YawSerialTask.cpp
 *  Sends data over serial communication to frontend, @c PlanarizerDAQFrontEnd.mlx
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/Yaw_MCU/YawSerialTask.cpp
 *  @author  Aaron Tran
 *  @date    2022-Jan-18 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "YawSerialTask.h"
#include "Yawshares.h"
#include "SoftwareSerial.h"

#define Rx1 PA10
#define Tx1 PA9

void task_serial(void* p_params)
{
    (void) p_params;

    ///@brief State of task_serial finite state machine
    uint8_t state = 0;
    ///@brief Incoming byte from serial communication
    uint8_t incoming = 0;
    ///@brief Delay value
    uint8_t delay_val = 50;     
    ///@brief Yaw encoder data
    float yaw_pos = 0;    
    ///@brief Time of each data collection
    unsigned long time = 0;    
    ///@brief Yaw checksum
    float yawcrc = 0;   
    ///@brief character to hold received data
    char recv_ch = 0;  

    Serial.begin (115200);
    
    for(;;)
        {
            data_state.get(state);         
            if(state==0)
            {
                delay_val = 50; //No need to call this task as much when waiting for 'begin' signal from frontend
            }
            else if(state==1) //Send data to frontend
            {
                delay_val = 1;
                for(uint8_t char_ct=0; char_ct<30;char_ct++)
                {
                    serial_queue.get(recv_ch);
                    Serial << recv_ch;
                }                       
            }
            else if(state ==2)
            {
                delay_val = 50; //No need to call this task as much when doing ping tests               
            }            
            vTaskDelay(delay_val);
        }

}
