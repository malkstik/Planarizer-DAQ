/** @file main.cpp
 *  This file contains a program for measuring the pitch ticks and sending it over to the Yaw MCU
 *  
 *  Source code available here:
 *  @author  Aaron Tran
 *  @date    24-Nov-2021 
 */
#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif

#include "taskshare.h"         // Header for inter-task shared data
#include "taskqueue.h"         // Header for inter-task data queues
#include "Pitchshares.h"            // Header for shares used in this project

#include "PitchBluetoothTask.h"
#include "PitchDataTask.h"


//Queue declarations
/// @brief Pitch data
Queue<float> pitch(50, "pitch");      

/// @brief Pitch data
Queue<float> time_data(50, "pitch");      

/// @brief Checksum
Queue<float> crc(50, "crc");   

/// @brief Data_state
Share<uint8_t> data_state("state");

/// @brief Data_state
Share<unsigned long> first_time("zero time");



void setup () 
{
    // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet
    Serial.begin (115200);
    delay (2000);
    Serial << endl << endl << "Initializing Pitch MCU" << endl;
    // Create a task which sends design data
    // This task would also collect data and write it to a CSV file upon further development 
    xTaskCreate (task_data,
                 "data",
                 4096,                            // Stack size
                 NULL,
                 5,                               // Priority
                 NULL);
    xTaskCreate (task_bluetooth,
                 "bluetooth",
                 4096,                            // Stack size
                 NULL,
                 2,                               // Priority
                 NULL);                 

    data_state.put(0);
    first_time.put(0);

    #if (defined STM32L4xx || defined STM32F4xx)
        vTaskStartScheduler ();
    #endif
}

void loop() 
{
}