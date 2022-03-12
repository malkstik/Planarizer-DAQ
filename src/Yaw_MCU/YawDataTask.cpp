/** @file YawDataTask.cpp
 *  This task is responsible for reading the encoder and sending the data in a queue to @c YawSerialTask.cpp
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/Yaw_MCU/YawDataTask.cpp
 *  @author  Aaron Tran
 *  @date    2022-Jan-17 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "YawDataTask.h"
#include "EncoderDriver.h"
#include "Yawshares.h"
#include <math.h>

//Encoder Pins
/// @brief Yaw Encoder Channel A
#define CHA PA0
/// @brief Yaw Encoder Channel B
#define CHB PA1

void task_data(void* p_params)
{
    (void) p_params;
    ///@brief State of task_data finite state machine
    uint8_t state = 0;
    ///@brief Start time of data collection
    unsigned long first_time = 0;  
    ///@brief Time of each data collection
    unsigned long time = 0;
    ///@brief Delay value
    uint8_t delay_val = 0;    
    ///@brief Yaw encoder data
    float yaw_pos = 0;

    // Drivers
    STM32Encoder yawENC (TIM2, CHA, CHB);
    for(;;)
    {
        zero_time.get(first_time);
        data_state.get(state);
        if(state==0)
        {
            delay_val = 5;
        }
        else if(state==1)
        {
            //Make sure the whole thing gets in there
            delay_val = 15;            
            time = millis() - first_time;
            yaw_pos = cos(time);  //For testing Serial Comm w/o encoder
            
            serial_queue << "Yaw," << yaw_pos << "," << time << "," << time + yaw_pos <<endl;
        }
        else if(state ==2)
        {
            delay_val = 50; //No need to call this task as much when doing ping tests               
        }              
        vTaskDelay(delay_val);
    }

}
