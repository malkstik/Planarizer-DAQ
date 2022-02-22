/** @file main.cpp
 *  This file contains a program for running the Planarizer DAQ.
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
#include "shares.h"            // Header for shares used in this project

#include "SerialTask.h"
#include "DataTask.h"


//Queue declarations
/// @brief Yaw data
Queue<float> yaw(50, "yaw");

/// @brief Pitch data
Queue<float> pitch(50, "pitch");      

/// @brief Pitch data
Queue<float> time_data(50, "pitch");      

/// @brief Data_state
Share<uint8_t> data_state("state");

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
                 3,                               // Priority
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

//Encoder Testing
/*
//Encoder Pins
#define E1CHA PA0
#define E1CHB PA1
#include "EncoderDriver.h"
void setup()
{ 
  //Test
  Serial.begin (115200);
  delay(5000);

  //Setup encoders

  Serial <<"Setting up encoder 1!" << endl;
  STM32Encoder enc1 (TIM2, E1CHA, E1CHB);

  enc1.zero();

  float x_pos;

  for(;;)
  {
  delay(2500);  
  x_pos = -enc1.update()*1.571/4000;
  Serial <<"Motor 1 Position:" << x_pos << "inches" << endl;

  //Serial <<"Motor 1 Position:" << enc1.update()*360/4000 << "degrees" << endl;
  //Serial <<"Motor 2 Position:" << enc2.update()*360/4000 << "degrees" << endl;
  //Serial <<"Motor 1 delta:" << enc1.update() << "ticks" << endl;
  //Serial <<"Motor 1 Position:" << enc1.delta << "ticks" << endl;
  //Serial <<"Motor 2 delta:" << enc2.update() << "ticks" << endl;
  //Serial <<"Motor 2 Position:" << enc2.delta << "ticks" << endl;
  }
}
*/
void loop() 
{
}