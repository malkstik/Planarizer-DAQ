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
    uint8_t state = 1;
    ///@brief Incoming byte from serial communication
    uint8_t incoming = 0;
    ///@brief Delay value
    uint16_t delay_val = 0;
    ///@brief Incoming line from serial communication
    char *str;
    ///@brief Time of each data collection
    unsigned long time = 0;
    ///@brief Pitch encoder data
    float pitch_pos = 0;
    ///@brief checksum
    float crc_now = 0;
    ///@brief Counter
    uint8_t ct = 0;      
    ///@brief UART pins for bluetooth
    HardwareSerial MyBlue(Rx1, Tx1); // RX | TX 

    unsigned long ping_time = millis();


    Serial.begin(115200);
    MyBlue.begin(230400);
    for(;;)
        {
            data_state.get(state);
            /*
            if (state ==0) //Determining round trip time 
            {
                delay_val = 10;
                if (MyBlue.available() > 0)
                {
                    Serial << "I got something!" << endl;
                    str = receiveLine(MyBlue.read());
                    if (str!=NULL)
                    {
                        MyBlue << str << '\r';
                        ct+=1;
                        Serial << "I saw this: " << str << endl;
                    }

                }
                if (ct >= 24)
                {
                    data_state.put(1);
                }
            }
            */
            if(state==0) //Wait to begin data collection
            {
                delay_val = 1;
                if (MyBlue.available() > 0)
                {
                    incoming = MyBlue.read();
                }
                if ((char)incoming == 'g') //If 'g' is received, change states to begin data collection
                {
                    first_time.put(millis());
                    data_state.put(1);
                }           
                else if ((char)incoming == 'p')
                {
                    data_state.put(2);
                    ping_time = millis();
                } 
            }
            else if(state==1) //Send data to Yaw MCU
            {
                delay_val = 20;
                pitch_pos = pitch.get(); time = time_data.get(); crc_now = crc.get();
                Serial <<  pitch_pos << ':' << time << ':' << crc_now << endl;
                MyBlue <<  pitch_pos << ':' << time << ':' << crc_now << '\r';
                if (MyBlue.available() > 0)
                {
                    incoming = MyBlue.read();
                }
                if ((char)incoming == 'r') //If 'r' is received, change states to avoid unnecessarey data collection and wait for next test
                {
                    data_state.put(0);
                }    
            }
            else if(state ==2)
            {
                delay_val = 1; //run at same speed as state 0 to better simulate circumstance
                if (millis() - ping_time > 1000)
                {
                    ping_time = millis();
                    Serial << "state:" << state << endl;                    
                }
                if (MyBlue.available()>0)
                {
                    incoming = MyBlue.read(); //read the data to get it out of the buffer
                    
                    MyBlue << "i"; //send something back so we can get an rx reading on the other side

                    Serial << (char)incoming << endl; //so we can check in PuTTy that we received something
                }
                if (Serial.available()>0)
                {
                    incoming = Serial.read();
                }
                if (char(incoming) == 's')
                {
                    data_state.put(0);
                }

            }
            vTaskDelay(delay_val);
        }

}