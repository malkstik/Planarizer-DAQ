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

    MyBlue.begin(230400);
    Serial.begin(115200);
    
    Serial << "YawBluetoothTask initialized" << endl;

    for(;;)
        {
            //data_state.get(state);
            state =1;
            if(state==0)
            {
                Serial << "I do stuff too!" << endl;
                delay_val = 5;
                if (Serial.available() > 0)
                {
                    incoming = Serial.read();
                    Serial << "saw this:" << (char)incoming << endl;
                }
                if ((char)incoming == 'g') //switch to data collection mode and tell Pitch MCU to switch to data collection mode
                {
                    data_state.put(1);
                    MyBlue.write('g'); //Hopefully one of these goes through
                    MyBlue.write('g');
                    MyBlue.write('g');
                    incoming = 0;
                }     
            }
            else if(state==1) //Read 
            {
                delay_val = 5;
                if (MyBlue.available()>0)
                {
                    str = receiveLine(MyBlue.read()); // Should receive pitch, pitch_time, pitch_crc
    
                    while (str == NULL) //Read the whole line instead of just a character
                    {
                        str = receiveLine(MyBlue.read());
                        //Serial << "checking the line" << endl;
                    }
                    if (str != NULL) //After the whole line has been read check if it was a signal to go back to state 0 or if it;s data
                    {
                        if (*str == 'r')
                        {
                            data_state.put(0);
                            MyBlue << 'r' << endl; //send 'r' multiple times in case if pitch MCU misses one
                            MyBlue << 'r' << endl;
                            MyBlue << 'r' << endl;
                        }
                        else
                        {
                            Serial << "Pitch:" << str << endl; //send data to frontend
                        }
                        
                    }

                }
            }
            vTaskDelay(delay_val);
        }
}