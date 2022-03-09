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
#include "SoftwareSerial.h"
#include <string>

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
    ///@brief String representing pitch
    std::string pitch_str;
    ///@brief String representing pitch_time
    std::string pitchtime_str;
    ///@brief String representing pitch_crc
    std::string pitchcrc_str;        
    ///@brief counter
    uint8_t ct = 0;
    ///@brief Callibration flag
    uint8_t call = 0;
    ///@brief UART pins for bluetooth
    SoftwareSerial MyBlue(Rx1, Tx1); // RX | TX 
    
    MyBlue.begin(9600);
    Serial.begin(115200);
    
    for(;;)
        {
            data_state.get(state);
            if (state ==0)
            {
                if (Serial.available() > 0)
                {
                    incoming = Serial.read();
                }
                if ((char)incoming == 'g') //switch to data collection mode and tell Pitch MCU to switch to data collection mode
                {
                    call = 1;
                    incoming = 0;
                }    
                if (ct <= 24 & call == 1) //send ping
                {
                    MyBlue << micros() << '\r'; //Send carriage return because receiveLine() uses it to detect the end of a line
                }
                if (MyBlue.available() > 0) //receive pings
                {
                    str = receiveLine(MyBlue.read());
                    Serial << "RTT:" << str << ":" << millis() << endl; // RTT:initial_ping_time: return_ping_time  frontend uses ':' for string separation
                    Serial << "Ct:" << ct << endl;
                    ct+=1;
                }
                if (ct >= 24)
                {
                    data_state.put(1);
                    call = 0;
                }
            }
            if(state==1)
            {
                if (Serial.available() > 0)
                {
                    incoming = Serial.read();
                }
                if ((char)incoming == 'g') //switch to data collection mode and tell Pitch MCU to switch to data collection mode
                {
                    data_state.put(2);
                    MyBlue.write('g');
                    incoming = 0;
                }         
           }
            else if(state==2) //Read 
            {
                if (MyBlue.available()>0)
                {
                    str = receiveLine(MyBlue.read()); // Should receive pitch, pitch_time, pitch_crc
                    if (str != NULL)
                    {
                        substr = strtok(str, ",");
                        if (substr[0] == 'g')
                        {
                            data_state.put(0);
                        }
                        else
                        {

                            pitch_str = substr[0]; 
                            pitchtime_str = substr[1];
                            pitchcrc_str = substr[2];
                            pitch.put(std::stof(pitch_str));
                            pitch_time.put(std::stof(pitchtime_str));
                            pitch_crc.put(std::stof(pitchcrc_str));
                        }
                        
                    }

                }

            }
            vTaskDelay(5);
        }
}