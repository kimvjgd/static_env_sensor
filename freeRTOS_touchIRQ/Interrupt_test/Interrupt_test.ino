#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"
#include "soc/wdev_reg.h"
#include "xtensa/core-macros.h"

#define TIRQ_PIN 35

//bool msg = false;
//void IRAM_ATTR Int_Test()
//{
//    msg = true;    
//}

void TouchIRQ_task(void *pvParameter)
{
    (void)pvParameter;

    pinMode(TIRQ_PIN, INPUT_PULLUP);
//    attachInterrupt(TIRQ_PIN, Int_Test, FALLING);

    while(1)
    {
        if(digitalRead(TIRQ_PIN) == false)
        {
            Serial.println("Interrupt");
        }
        
        vTaskDelay(90 / portTICK_RATE_MS);
    }
}

// #define LED_PIN     2
// #define LED_ON      digitalWrite(LED_PIN, HIGH)
// #define LED_OFF     digitalWrite(LED_PIN, LOW)

// void LEDToggle_task(void *pvParameter)
// {
//     (void)pvParameter;

//     pinMode(LED_PIN, OUTPUT);

//     while(1)
//     {
//         LED_ON;
//         delay(100);
//         LED_OFF;
//         delay(100);
        
//         vTaskDelay(1 / portTICK_RATE_MS);
//     }
// }

void freeRTOS_setup()
{    
    xTaskCreatePinnedToCore(&TouchIRQ_task, "TouchIRQ_task", 1024, NULL, 5, NULL, ARDUINO_RUNNING_CORE);
    // xTaskCreatePinnedToCore(&LEDToggle_task, "LEDToggle_task", 1024, NULL, 6, NULL, ARDUINO_RUNNING_CORE);
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);

    freeRTOS_setup();
    
    

    Serial.println("Interrutpt Test");
}

void loop() 
{
//    if(msg)
//    {
//        if(digitalRead(TIRQ_PIN) == false)
//        {
//            Serial.println("Interrupt");
//        }
//        else
//        {
//            msg = false;
//        }
//    }
}
