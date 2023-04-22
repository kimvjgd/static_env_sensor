#include <XPT2046_Touchscreen.h>
#include <SPI.h>

#define RXD2 16
#define TXD2 17

#define CS_PIN  5

#define TIRQ_PIN  35
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);





void TOUCHIRQ_task(void *pvParameter){
  (void)pvParameter;

    pinMode(TIRQ_PIN, INPUT_PULLUP);
//    attachInterrupt(TIRQ_PIN, Int_Test, FALLING);
    
    ts.begin();
    ts.setRotation(1);
    while(1)
    {
        if(ts.touched()){
          TS_Point p = ts.getPoint();          
          Serial2.print(p.y);
          Serial2.print(",");
          Serial2.println(p.x);     
          Serial.print(p.y);
          Serial.print(",");
          Serial.println(p.x);  
        }
        
        vTaskDelay(100 / portTICK_RATE_MS);
    }  
}

void freeRTOS_setup()
{    
    xTaskCreatePinnedToCore(&TOUCHIRQ_task, "TOUCHIRQ_task", 1024, NULL, 5, NULL, ARDUINO_RUNNING_CORE);
    // xTaskCreatePinnedToCore(&LEDToggle_task, "LEDToggle_task", 1024, NULL, 6, NULL, ARDUINO_RUNNING_CORE);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  freeRTOS_setup();  
}

void loop() {
  // put your main code here, to run repeatedly:

}
