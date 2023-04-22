///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// SVM30 ////////////////////////////////////////////////////////////////////
#include <svm30.h>
#include <Arduino.h>

// SVM30 Init Declaration //
/*
   SVM30               ESP32
   1. SDL  ----lll-----  SCL (pin 22)
   2. GND  -----------   GND
   3. VCC  -----------   +5V/VUSB
   4. SDA  ----lll-----  SDA (pin 21)
*/

#define DEBUG false
#define DELAY 5


// create instance
SVM30 svm;

void Errorloop(char *mess)
{
  Serial.println(mess);
  Serial.println(F("Program on hold"));
  for (;;) delay(100000);
}

void KeepTrigger(uint8_t del)
{
  uint8_t w_seconds = del;
  unsigned long startMillis;

  if (w_seconds == 0) w_seconds = 1;

  while (w_seconds--)
  {
    startMillis = millis();

    if (! svm.TriggerSGP30())
      Errorloop((char *) "Error during trigger waiting");

    // this gives 1Hz /1000ms (aboutisch)
    while (millis() - startMillis < 1000);
  }
}

/*
   @brief : read and display the values from the SVM30

*/
void read_values() 
{
    struct svm_values v;

    if (! svm.GetValues(&v))
    {
        Errorloop((char *) "Error during reading values");
    }
    else
    {  
        Serial.println("svm30 Measurement");
        Serial.print(F("CO2 equivalent : "));
        Serial.print(v.CO2eq);
      
        Serial.print(F(", TVOC : "));
        Serial.print(v.TVOC);
      
        Serial.print(F(", H2_signal : "));
        Serial.println(v.H2_signal);
      
        Serial.print(F(", Ethanol_signal : "));
        Serial.print(v.Ethanol_signal);
      
        Serial.print(F(", Humidity : "));
        Serial.println((float) v.humidity / 1000);
      
        Serial.print(F(", temperature : "));
        Serial.print((float) v.temperature / 1000);
      
        Serial.print(F(", absolute humidity : "));
        Serial.println(v.absolute_hum);
    }
}

/*
   @brief: read and display the id of the SGP30 and SHTC1 sensors
*/
void read_id() 
{
    uint16_t buf[3];    // SGP30 needs 3 words, SHTC1 is 1 word
    char  id[15];

    if ( ! svm.GetId(SGP30, buf))
    {
        Errorloop((char *) "could not read SGP30 id");
    }

    Serial.print(F("\nSGP30 id : "));
    sprintf(id, "%04x %04x %04x", buf[0], buf[1], buf[2]);
    Serial.println(id);

    if (svm.GetId(SHTC1, buf) == false)
    {
        Errorloop((char *) "could not read SHTC1 id");
    }

    Serial.print(F("SHTC1 id : "));
    // only bit 5:0 matter (source: datasheet)
    sprintf(id, "%04x", buf[0] & 0x3f);
    Serial.println(id);
}

/*
   @brief: read and display the product feature set of the SGP30 sensor
*/
void read_featureSet() 
{
    char buf[2];

    if ( ! svm.GetFeatureSet(buf))
    {
        Errorloop((char *) "could not read SGP30 feature set");
    }
    else
    {
        Serial.print(F("\nSGP30 product type : "));
        Serial.print((buf[0] & 0xf0), HEX);
      
        Serial.print(F(", Product version : "));
        Serial.println(buf[1], HEX);
        Serial.println();
    }
}

/*
   @brief: read the baselines of the SGP30 sensor
   see example3 for extended information about baselines
*/
void read_baseline() 
{
    uint16_t baseline;

    if (! svm.GetBaseLine_CO2(&baseline))
    {
        Errorloop((char *) "could not read SGP30 CO2 baseline");
    }
    else
    {
        Serial.print(F(" CO2 equivalent baseline : 0x"));
        Serial.print(baseline, HEX);
      
      //  if (! svm.GetBaseLine_TVOC(&baseline))
      //    Errorloop((char *) "could not read SGP30 TVOC baseline");
      
        Serial.print(F(", TVOC baseline : 0x"));
        Serial.println(baseline, HEX);
    }
}

/**
    @brief : continued loop after fatal error
    @param mess : message to display
*/

///////////////////////////////////////////////////



void svm30_setup() {
  // enable debug messages
  svm.EnableDebugging(DEBUG);

  svm.begin();

  Serial.print(F("Driver version : "));
  Serial.println(svm.GetDriverVersion());

  // try to detect SVM30 sensors
  if (svm.probe() == false) Errorloop((char *) "could not detect SVM30 sensors");
  else Serial.println(F("SVM30 detected"));

  // read and display the ID
  read_id();

  // read SGP30 feature set
  read_featureSet();
}

void svm30_loop() {
  // read measurement values
  read_values();

  // read SGP30 baseline
  read_baseline();

  // wait x seconds
  // KeepTrigger(DELAY);
}

//////////////////////////////////////////////////////////// SVM30 ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
