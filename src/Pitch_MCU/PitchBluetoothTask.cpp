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
    ///@brief Delay value
    uint8_t delay_val = 0;
    ///@brief Incoming line from serial communication
    char *str;
    ///@brief Array of str after being separated using @c strtok()
    char *substr;
    ///@brief Counter
    uint8_t ct = 0;
    ///@brief UART pins for bluetooth
    SoftwareSerial MyBlue(Rx1, Tx1); // RX | TX 
    MyBlue.begin(9600);
    
    for(;;)
        {
            data_state.get(state);
            if (state ==0) //Determining round trip time 
            {
                delay_val = 1;
                if (MyBlue.available() > 0)
                {
                    str = receiveLine(MyBlue.read());
                    MyBlue << str << '\r';
                    ct+=1;
                }
                if (ct >= 24)
                {
                    data_state.put(1);
                }

            }
            else if(state==1) //Wait to begin data collection
            {
                delay_val = 1;
                if (MyBlue.available() > 0)
                {
                    incoming = MyBlue.read();
                }
                if ((char)incoming == 'g') //If 'g' is received, change states to begin data collection
                {
                    data_state.put(2);
                }           
            }
            else if(state==2) //Send data to Yaw MCU
            {
                delay_val = 5;
                Serial <<  pitch.get() << ',' << time_data.get() << ',' << crc.get() << endl;
                if (MyBlue.available() > 0)
                {
                    incoming = MyBlue.read();
                }
                if ((char)incoming == 'g') //If 'g' is received, change states to avoid unnecessarey data collection and wait for next test
                {
                    data_state.put(0);
                }           
            vTaskDelay(delay_val);
            }

        }

}