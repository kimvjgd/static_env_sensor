#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"
#include "soc/wdev_reg.h"
#include "xtensa/core-macros.h"

#include "sps30.h"
#include "svm30.h"
#include "ze08.h"
#include "s300i2c.h"
#include "adc_dac.h"

#include <DFRobot_VEML7700.h>

#include <XPT2046_Touchscreen.h>
#define RXD2 16
#define TXD2 17
#define CS_PIN  5
#define TIRQ_PIN 35
// For Mux
const byte sel0 = 4;
const byte sel1 = 27;


S300I2C s3(Wire);

XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);

DFRobot_VEML7700 light_sensor;

HardwareSerial mySerial(2);

void TouchIRQ_task(void *pvParameter)
{
    (void)pvParameter;

    pinMode(TIRQ_PIN, INPUT_PULLUP);
//    attachInterrupt(TIRQ_PIN, Int_Test, FALLING);

    while(1)
    {
        if(digitalRead(TIRQ_PIN) == false)
        {
            if(ts.touched()){
                TS_Point p = ts.getPoint();
                mySerial.print(p.y);
                mySerial.print(",");
                mySerial.println(p.x);
                Serial.print(p.y);
                Serial.print(',');
                Serial.println(p.x);
                
                
            }
        }
        
        vTaskDelay(90 / portTICK_RATE_MS);
    }
}

void freeRTOS_setup()
{    
    xTaskCreatePinnedToCore(&TouchIRQ_task, "TouchIRQ_task", 1024, NULL, 5, NULL, ARDUINO_RUNNING_CORE);
    // xTaskCreatePinnedToCore(&LEDToggle_task, "LEDToggle_task", 1024, NULL, 6, NULL, ARDUINO_RUNNING_CORE);
}

void init_setting(){

    // s300 serial communication specification is 115200 bps
    // it should be only 115200 for s300 sensor
    Serial.begin(115200);
    // 여기서 안되면 115200을 9600으로 바꿔줘야 한다.
    mySerial.begin(115200, SERIAL_8N1, 16, 17);

    freeRTOS_setup();

    Wire.begin();

    pinMode(sel0, OUTPUT);
    pinMode(sel1, OUTPUT);
    // digitalWrite(sel0, 1);
    // digitalWrite(sel1, 0);            
    light_sensor.begin();
    sps30_setup();
    svm30_setup();

    s3.begin(S300I2C_ADDR);
    s3.wakeup();
    s3.end_mcdl();
    s3.end_acdl();
    pinMode(NH3_Heat_PIn, OUTPUT);
    digitalWrite(NH3_Heat_PIn, HIGH);
    ADC_DAC_Init();
    
    // For Touch
    digitalWrite(sel0, 0);
    digitalWrite(sel1, 0);
    ts.begin();
    ts.setRotation(1);
    

    delay(5000);
}





void mux_change(int m)            // (X0 - OPi)    (X1 - ZE08)   (X2 - RS9A)
{
    if(m==0)
    {           // 0 for ze08
        digitalWrite(sel1, 0);
    }
    else 
    {             // 1 for gps
        digitalWrite(sel1, 1);
    }
}