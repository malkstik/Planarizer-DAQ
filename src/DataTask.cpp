/** @file DataTask.cpp
 *  This class is responsible for reading the encoder and sending the data in a queue to @c SerialTask.cpp
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/DataTask.cpp
 *  @author  Aaron Tran
 *  @date    2022-Jan-17 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "DataTask.h"
#include "EncoderDriver.h"
#include "shares.h"
#include <math.h>

//Encoder Pins
/// @brief Yaw Encoder Channel A
#define E1CHA PA0
/// @brief Yaw Encoder Channel B
#define E1CHB PA1
/// @brief Pitch Encoder Channel A
#define E2CHA PB4
/// @brief Pitch Encoder Channel B
#define E2CHB PB5

void task_data(void* p_params)
{
    (void) p_params;
    ///@brief State of task_data finite state machine
    uint8_t state = 0;
    ///@brief Start time of data collection
    unsigned long first_time = 0;  
    ///@brief Time of each data collection
    unsigned long time = 0;
    ///@brief Yaw encoder data
    float yaw_pos = 0;
    ///@brief Pitch encoder data
    float pitch_pos = 0;
    ///@brief Incoming byte from serial communication
    uint8_t incoming = 0;

    // Drivers
    STM32Encoder yawENC (TIM2, E1CHA, E1CHB);
    STM32Encoder pitchENC (TIM3, E2CHA, E2CHB);
    data_state.put(0);
    for(;;)
    {
        Serial.begin(115200);
        data_state.get(state);
        if(state==0)
        {
            first_time = millis();
            if (Serial.available() > 0)
            {
                incoming = Serial.read();
                //Serial.println(incoming, DEC);
            }
            Serial << "I saw this:" << (char)incoming << endl;
            if (incoming == 'g')
            {
                data_state.put(1);
            }
        }
        else if(state==1)
        {
            //For testing Serial Comm w/o encoder
            /*
            yaw_pos = 360/40000*yawENC.update();
            //pitch_pos = 360/40000*pitchENC.update();
            */
            time = millis() - first_time;
            yaw_pos = cos(time);
            pitch_pos = sin(time);

            yaw.put(yaw_pos);
            pitch.put(pitch_pos);
            time_data.put(time);
        }
        vTaskDelay(250);
    }

}
