#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "SerialTask.h"
#include "shares.h"


void task_serial(void* p_params)
{
    (void) p_params;

    uint8_t state = 0;
    Serial.begin (115200);
    for(;;)
        {
            if(state==0)
            {
            //Serial << "Pitch: "<< pitch.get() << "deg" << endl;
            Serial << "Yaw: " << yaw.get() << "deg" << endl;  
            Serial << "Time: " << time_data.get() << "ms" << endl << endl;
            }
            vTaskDelay(100);

        }

}
