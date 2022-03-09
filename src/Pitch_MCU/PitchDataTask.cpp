/** @file PitchDataTask.cpp
 *  This class is responsible for reading the encoder and sending the data in a queue to @c PitchBluetoothTask.cpp
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/Pitch_MCU/PitchDataTask.cpp
 *  @author  Aaron Tran
 *  @date    2022-Jan-17 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "PitchDataTask.h"
#include "EncoderDriver.h"
#include "Pitchshares.h"
#include <math.h>

//Encoder Pins
/// @brief Pitch Encoder Channel A
#define CHA PA0
/// @brief Pitch Encoder Channel B
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
    ///@brief Pitch encoder data
    float pitch_pos = 0;
    ///@brief Delay value
    uint8_t delay_val = 0;    
    ///@brief Incoming byte from serial communication with PC
    uint8_t incoming = 0;
    ///@brief Incoming byte from serial communication with Pitch MCU
    uint8_t incoming_pitch = 0;
    ///@brief checksum
    float crc_now = 0;

    // Drivers
    STM32Encoder pitchENC (TIM2, CHA, CHB);
    data_state.put(0);
    for(;;)
    {
        Serial.begin(115200);
        data_state.get(state);
        if (state ==0) //Callibrating bluetooth connection
        {
            delay_val = 50; //Don't call this task during bluetooth callibration
        }        
        else if(state==1) //Waiting to start data collection
        {
            delay_val = 1;
            first_time = micros();
        }
        else if(state==2) //Collect data
        {
            //For testing Serial Comm w/o encoder
            pitch_pos = sin(time);
            //pitch_pos = 360/40000*pitchENC.update();
            delay_val = 5;
            time = micros() - first_time;
            
            crc_now = time + pitch_pos;

            pitch.put(pitch_pos);
            time_data.put(time);
            crc.put(crc_now);
        }
        vTaskDelay(delay_val);
    }

}
