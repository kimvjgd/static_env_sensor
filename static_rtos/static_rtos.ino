#include "Application.h"

#include <Wire.h>

float lux;
unsigned int co2;


void setup() {
  init_setting();
}

/*
SVM30   -   TVOC, CO2, Temperature, Humidity
SPS30   -   PM2.5, PM10
MICS    -   NH3
ZE08    -   CH2O
RS9A    -   RN
SKU1010 -   Sound
VEML    -   Light
110-102 -   CO
110-303 -   H2S
110-406 -   O3
110-507 -   NO2
*/

void loop() {
  sps30_loop();
  delay(100);
  svm30_loop();
  delay(100);
  // 얘가 잡고 있을때는 안된다.
  while(!ze08_loop(mySerial));

  // s3
  co2 = s3.getCO2ppm();
  Serial.print("s300 measurement - CO2 : ");
  Serial.println(co2);
  
  light_sensor.getALSLux(lux);
  Serial.print("Lux:");
  Serial.print(lux);
  Serial.println(" lx");

  ADC_DAC_Test();     // ++ => set this code to FreeRTOS later

  delay(1000);  
}
