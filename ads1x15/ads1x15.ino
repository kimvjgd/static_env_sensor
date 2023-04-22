#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

void ADC_Init()
{
    Wire.begin();
    
    ads.begin();

}

void Read_ADS1115(int ch)
{
   int data = ads.readADC_SingleEnded(ch);
   //float volt = ads.computeVolts(data) * 1000;
   float volt = ads.computeVolts(data);
   
   Serial.print("CH");
   Serial.print(ch);
   Serial.print(" : ");
    Serial.println(volt);
    // delay(100);
}

void setup()
{
    Serial.begin(115200);
    ADC_Init();
}

void loop()
{
    for(int i = 0; i< 4; i++)
    {
      //Read_ADS1115(i);
    }
    Read_ADS1115(3);
    // delay(1000);
}