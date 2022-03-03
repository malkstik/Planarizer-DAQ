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
    ///@brief UART pins for bluetooth
    SoftwareSerial MyBlue(Rx1, Tx1); // RX | TX 
    
    MyBlue.begin(9600);
    Serial.begin (115200);
    
    for(;;)
        {
            data_state.get(state);
            if(state==0)
            {
                //pass
            }
            else if(state==1)
            {
                Serial << "Pitch:"<< pitch.get() << endl;
                Serial << "Yaw:" << yaw.get() << endl;  
                Serial << "Time:" << yaw_time.get() << endl;
            }
            vTaskDelay(5);

        }

}
