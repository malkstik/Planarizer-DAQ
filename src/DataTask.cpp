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
    float x_pos = 0;
    float z_pos = 0;

    STM32Encoder xENC (TIM2, E1CHA, E1CHB);
    STM32Encoder zENC (TIM3, E2CHA, E2CHB);

    for(;;)
    {
        if(state==0)
        {
            first_time = millis();
            state = 1;
        }
        elif(state==1)
        {
            x_pos = xENC.update();
            z_pos = zENC.update();
            time = millis() - first_time;
        }
        vTaskDelay(5);
    }

}