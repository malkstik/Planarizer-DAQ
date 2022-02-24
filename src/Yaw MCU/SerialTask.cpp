/** @file SerialTask.cpp
 *  Sends data over serial communication to frontend
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/SerialTask.cpp
 *  @author  Aaron Tran
 *  @date    2022-Jan-18 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "SerialTask.h"
#include "shares.h"


void task_serial(void* p_params)
{
    (void) p_params;

    ///@brief State of task_serial finite state machine
    uint8_t state = 0;
    Serial.begin (115200);
    for(;;)
        {
            data_state.get(state);
            if(state==0)
            {
                //Don't do anything in this state
            }
            else if(state==1)
            {
                Serial << "Pitch:"<< pitch.get() << endl;
                Serial << "Yaw:" << yaw.get() << endl;  
                Serial << "Time:" << time_data.get() << endl;
            }
            vTaskDelay(5);

        }

}
