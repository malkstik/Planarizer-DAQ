/** @file BluetoothTask.cpp
 *  This class is responsible for reading the encoder and sending the data in a queue to @c SerialTask.cpp
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/BluetoothTask.cpp
 *  @author  Aaron Tran
 *  @date    2022-Feb-28 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "shares.h"
#include "SoftwareSerial.h"

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
    ///@brief Incoming line from serial communication
    char *str;
    ///@brief Array of str after being separated using @c strtok()
    char *substr;
    ///@brief UART pins for bluetooth
    SoftwareSerial MyBlue(Rx1, Tx1); // RX | TX 
    
    MyBlue.begin(9600);
    Serial.begin(115200);
    
    for(;;)
        {
            data_state.get(state);
            if(state==0)
            {
                if (Serial.available() > 0)
                {
                    str = receiveLine(Serial.read());
                    //Serial.println(incoming, DEC);
                }
                //Serial << "I saw this:" << (char)incoming << endl;
                if ((char)str == 'g')
                {
                    data_state.put(1);
                    MyBlue.write('g');
                }            }
            else if(state==1)
            {
                if (MyBlue.available()>0)
                {
                    str = receiveLine(MyBlue.read()); // Should receive pitch, pitch_time
                    if (str != NULL)
                    {
                        substr = strtok(str, ",");
                        pitch.put(substr[0]);
                        pitch_time.put(substr[1]);
                    }
                }


            }
            vTaskDelay(5);

        }

}