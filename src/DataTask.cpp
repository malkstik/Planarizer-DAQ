#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "DataTask.h"
#include "EncoderDriver.h"
#include "shares.h"

//Encoder Pins
#define E1CHA PA0
#define E1CHB PA1

#define E2CHA PB4
#define E2CHB PB5

void task_data(void* p_params)
{
    (void) p_params;
    uint8_t state = 0;
    unsigned long first_time = 0;  
    unsigned long time = 0;
    float yaw_pos = 0;
    float pitch_pos = 0;

    STM32Encoder yawENC (TIM2, E1CHA, E1CHB);
    STM32Encoder pitchENC (TIM3, E2CHA, E2CHB);

    for(;;)
    {
        if(state==0)
        {
            first_time = millis();
            state = 1;
        }
        else if(state==1)
        {
            yaw_pos = yawENC.update();
            pitch_pos = pitchENC.update();
            time = millis() - first_time;

            yaw.put(yaw_pos);
            pitch.put(pitch_pos);
            time_data.put(time);

        }
        vTaskDelay(5);
    }

}