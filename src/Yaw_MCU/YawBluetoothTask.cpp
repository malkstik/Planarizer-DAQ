/** @file YawBluetoothTask.cpp
 *  This task is responsible for reading the encoder and sending the data in a queue to @c YawSerialTask.cpp
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/Yaw_MCU/YawBluetoothTask.cpp
 *  @author  Aaron Tran
 *  @date    2022-Feb-28 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "Yawshares.h"
#include "YawBluetoothTask.h"

#define Rx1 PA10
#define Tx1 PA9

#define SEPERATING_CHAR 13  // ASCII-13 is '\r' carriage return character that seperates two lines

/** @brief   Reads serial communication line, determines end by looking for '\r'
 * 
 *  @details This function sets reads a full line from the serial port instead of just one char.
 *           Found here @c https://forum.arduino.cc/t/robust-serial-read-with-delimiters/331603/7
 */
char* receiveLine(char c)
{
  static char lineBuffer[41];  // define maximum buffer length here (max string length +1)
  static byte counter=0;
  if (counter==0) memset(lineBuffer,0,sizeof(lineBuffer)); // clear buffer before using it
  if (c==SEPERATING_CHAR)
  {
    counter=0;
    return lineBuffer;
  }
  else if (c>=32 && counter<sizeof(lineBuffer)-1) 
  { // if is it a printable ASCII-character and fits into the lineBuffer
    lineBuffer[counter]=c; // insert character into lineBuffer
    counter++;
  }
  return NULL;
}

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
    ///@brief Incoming line from serial communication
    char *str;
    ///@brief Array of str after being separated using @c strtok()
    char *substr;
    
    ///@brief counter
    uint8_t ct = 0;
    ///@brief Callibration flag
    uint8_t call = 0;
    ///@brief Delay value
    uint8_t delay_val = 50;         
    ///@brief UART pins for bluetooth
    HardwareSerial MyBlue(Rx1, Tx1); // RX | TX 

    ///@brief flag triggered when starting data collection
    bool match_flag = false;
    ///@brief used to match the time that microcontroller zero their timers
    unsigned long match_time = millis();

    ///@brief used to periodically send pings in ping test mode
    unsigned long ping_time = millis();

    Serial.begin(115200);
    MyBlue.begin(230400);
    
    for(;;)
        {
            data_state.get(state);
            if(state==0)
            {
                delay_val = 1;
                if (Serial.available() > 0)
                {
                    incoming = Serial.read();
                }
                if ((char)incoming == 'g') //switch to data collection mode and tell Pitch MCU to switch to data collection mode
                {
                    MyBlue.write('g'); //Hopefully one of these goes through
                    MyBlue.write('g');
                    MyBlue.write('g');
                    match_time = millis();
                    match_flag = true;
                    incoming = 0;
                }
                else if ((char)incoming == 'p')
                {
                    data_state.put(2);
                    MyBlue.write('p'); //Hopefully one of these goes through
                    MyBlue.write('p');
                    MyBlue.write('p');        
                    ping_time = millis();   
                    incoming = 0;
                }
                if (match_flag)
                {
                    if (millis()-match_time >=6.9)
                    {
                        match_flag= false;
                        zero_time.put(millis());
                        data_state.put(1);
                    }
                }
            }
            else if(state==1) //Read 
            {
                delay_val = 15;
                if (MyBlue.available()>0)
                { 
                    str = receiveLine(MyBlue.read()); // Should receive pitch, pitch_time, pitch_crc
                    
                    while (str == NULL) //Read the whole line instead of just a character
                    {
                        str = receiveLine(MyBlue.read());
                    }
                    serial_queue << "Pitch," << str << endl; 
                }
                if (Serial.available() > 0)
                {
                    incoming = Serial.read();
                }                
                if ((char)incoming == 'r') //switch to data collection mode and tell Pitch MCU to switch to data collection mode
                {
                    data_state.put(0);    
                    MyBlue.write('r'); //Hopefully one of these goes through
                    MyBlue.write('r');
                    MyBlue.write('r');                                
                }
            }
            else if(state ==2)
            {   
                delay_val = 5;
                //send 'i' as ping once every second
                if (millis()-ping_time > 250)
                {
                    MyBlue << 'i'; 
                    ping_time = millis();
                }
                if (MyBlue.available()>0)
                {
                    incoming = MyBlue.read(); //read the data to get it out of the buffer
                    Serial << "received ping back";
                }
                if (Serial.available() > 0)
                {
                    incoming = Serial.read();
                }
                if (char(incoming) == 's')
                {
                    data_state.put(0);
                    MyBlue.write('s'); //Hopefully one of these goes through
                    MyBlue.write('s');
                    MyBlue.write('s');  
                }

            }
            

            vTaskDelay(delay_val);
            
         }
}