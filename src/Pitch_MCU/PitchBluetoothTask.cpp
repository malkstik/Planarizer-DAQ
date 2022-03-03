/** @file PitchBluetoothTask.cpp
 *  This task is responsible for sending data over to the Yaw MCU
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/Pitch_MCU/PitchBluetoothTask.cpp
 *  @author  Aaron Tran
 *  @date    2022-Feb-28 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "Pitchshares.h"
#include "SoftwareSerial.h"
#include "PitchBluetoothTask.h"

#define Rx1 PA10
#define Tx1 PA9

/** @brief   Reads serial communication over bluetooth from pitch MCU and stores data in queue
 * 
 */
void task_bluetooth(void* p_params)
{
    (void) p_params;

    ///@brief State of task_serial finite state machine
    uint8_t state = 0;
    ///@brief Incoming byte from serial communication
    uint8_t incoming = 0;
    ///@brief UART pins for bluetooth
    SoftwareSerial MyBlue(Rx1, Tx1); // RX | TX 
    
    extern TaskHandle_t xBluetooth;
    MyBlue.begin(9600);
    
    for(;;)
        {
            data_state.get(state);
            if(state==0)
            {
                if (MyBlue.available() > 0)
                {
                    incoming = MyBlue.read();
                    //Serial.println(incoming, DEC);
                }
                //Serial << "I saw this:" << (char)incoming << endl;
                if ((char)incoming == 'g')
                {
                    data_state.put(1);
                    vTaskPrioritySet( xBluetooth,  1);
                }            }
            else if(state==1)
            {
                Serial <<  pitch.get() << ', ' << time_data.get() << endl;
            }
            vTaskDelay(5);

        }

}