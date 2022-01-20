#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif

#include "taskshare.h"         // Header for inter-task shared data
#include "taskqueue.h"         // Header for inter-task data queues
#include "shares.h"            // Header for shares used in this project

#include "SerialTask.h"
#include "DataTask.h"
void setup () 
{
    // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet
    Serial.begin (115200);
    delay (2000);
    Serial << endl << endl << "Initializing Mechatronic Zen Garden" << endl;

    // Create a task which sends design data
    // This task would also collect data and write it to a CSV file upon further development 
    xTaskCreate (task_data,
                 "data",
                 4096,                            // Stack size
                 NULL,
                 2,                               // Priority
                 NULL);
    xTaskCreate (task_serial,
                 "data",
                 4096,                            // Stack size
                 NULL,
                 2,                               // Priority
                 NULL);                 
    #if (defined STM32L4xx || defined STM32F4xx)
        vTaskStartScheduler ();
    #endif
}

void loop() 
{
}