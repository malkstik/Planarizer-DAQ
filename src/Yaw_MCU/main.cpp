/** @file main.cpp
 *  This file contains a program for collecting yaw data, receiving pitch data, and sending both to the frontend
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
#include "Yawshares.h"            // Header for shares used in this project
#include "textqueue.h"

#include "YawSerialTask.h"
#include "YawDataTask.h"
#include "YawBluetoothTask.h"

//Queue declarations
/// @brief Yaw data
Queue<float> yaw(50, "yaw");
   
/// @brief Yaw time
Queue<float> yaw_time(50, "yaw_time");      

/// @brief Yaw checksum
Queue<float> yaw_crc(50, "yaw checksum");

/// @brief data to send through serial port
TextQueue serial_queue(30000, "Pitch data via BT");

/// @brief Data_state
Share<uint8_t> data_state("state");

/// @brief Time that microcontroller begins data collection
Share<unsigned long> zero_time("zero");

void setup () 
{
    // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet

    serial_queue << endl << endl << "Initializing Planarizer DAQ" << endl;
    
    xTaskCreate (task_data,
                 "data",
                 4096,                            // Stack size
                 NULL,
                 8,                               // Priority
                 NULL);
    xTaskCreate (task_serial,
                 "serial",
                 4096,                            // Stack size
                 NULL,
                 1,                               // Priority
                 NULL);
    xTaskCreate (task_bluetooth,
                 "bluetooth",
                 4096,                            // Stack size
                 NULL,
                 6,                               // Priority
                 NULL);                                      
    
    data_state.put(0); //Populate share to avoid blocking, may also be used for debugging specific states

    #if (defined STM32L4xx || defined STM32F4xx)
        vTaskStartScheduler ();
    #endif
}

void loop() 
{
}