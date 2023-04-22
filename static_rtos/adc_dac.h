#include "MCP4725.h"

#define DAC_CH_NO2      0
#define DAC_CH_CO       1
#define DAC_CH_H2S      2
#define DAC_CH_O3       3

#define SECOND_I2C_SDA 33
#define SECOND_I2C_SCL 32

#define  NH3_Heat_PIn    26

#include <Adafruit_ADS1X15.h>
TwoWire SECOND_WIRE = TwoWire(1);
struct SensorADC
{
    float VBAT;
    float BH3;
    float AUDIO;

    float O3;
    float H2S;
    float CO;
    float NH3;
};

SensorADC ADCSensors;

Adafruit_ADS1115 ads;
Adafruit_ADS1115 ads110;

MCP4725 MCP_DAC_NO2(0x60);
MCP4725 MCP_DAC_CO(0x61);
MCP4725 MCP_DAC_H2S(0x62, &SECOND_WIRE);
MCP4725 MCP_DAC_O3(0x63, &SECOND_WIRE);

void ADC_DAC_Init()
{
    // run in main Code
    // Wire.begin();
    
    SECOND_WIRE.begin(SECOND_I2C_SDA,SECOND_I2C_SCL, 100000);

    // DAC Init
    MCP_DAC_NO2.begin();
    MCP_DAC_CO.begin();
    MCP_DAC_H2S.begin();
    MCP_DAC_O3.begin();

    // ADC Init
    ads.begin();
    ads110.begin(0x49);
    pinMode(NH3_Heat_PIn, OUTPUT);
    digitalWrite(NH3_Heat_PIn, HIGH);
}

float MCP_DAC_OutVolt(int tar, float volt)
{
    int c_volt = 4096 / 5 * volt;

    switch(tar)
    {
        case DAC_CH_NO2:
            MCP_DAC_NO2.setValue(c_volt);
        break;
        case DAC_CH_CO:
            MCP_DAC_CO.setValue(c_volt);
        break;
        case DAC_CH_H2S:
            MCP_DAC_H2S.setValue(c_volt);
        break;
        case DAC_CH_O3:
            MCP_DAC_O3.setValue(c_volt);
        break;
    }
    return c_volt;
}

#define ADC_CH_VBAT     0
#define ADC_CH_BH3      1
#define ADC_CH_AUDIO    2
#define ADC_CH_O3       3
#define ADC_CH_H2S      4
#define ADC_CH_CO       5
#define ADC_CH_NH3      6

float Read_ADS1115(int tar)
{
    int dint;
    float dfloat;
    switch(tar)
    {
        case ADC_CH_VBAT:
            dint = ads.readADC_SingleEnded(1);
            break;
        case ADC_CH_BH3:
            dint = ads.readADC_SingleEnded(2);
            break;
        case ADC_CH_AUDIO:
            dint = ads.readADC_SingleEnded(3);
            break;
        case ADC_CH_O3:
            dint = ads110.readADC_SingleEnded(0);
            break;
        case ADC_CH_H2S:
            dint = ads110.readADC_SingleEnded(1);
            break;
        case ADC_CH_CO:
            dint = ads110.readADC_SingleEnded(2);
            break;
        case ADC_CH_NH3:
            dint = ads110.readADC_SingleEnded(3);
            break;
    }
    dfloat = ads.computeVolts(dint);
    return dfloat;
}

// void setup() {
//   // put your setup code here, to run once:
//     Serial.begin(115200);
    
//     ADC_DAC_Init();
    
//     pinMode(NH3_Heat_PIn, OUTPUT);
//     digitalWrite(NH3_Heat_PIn, HIGH);
// }

int O3_cnt = 0;
int H2S_cnt = 1000;
int CO_cnt = 2000;
int NH3_cnt = 3000;
void ADC_DAC_Test()
{
    ADCSensors.VBAT = Read_ADS1115(ADC_CH_VBAT);
    ADCSensors.BH3 = Read_ADS1115(ADC_CH_BH3);
    ADCSensors.AUDIO = Read_ADS1115(ADC_CH_AUDIO);
    Serial.print("VBAT(");
    Serial.print(ADCSensors.VBAT);
    Serial.print(")\tBH3(");
    Serial.println(ADCSensors.BH3);
    Serial.print(")\tAUDIO(");
    Serial.print(ADCSensors.AUDIO);
    Serial.println(")");

    O3_cnt++;
    if(O3_cnt > 4095)
    {
        O3_cnt = 0;
    }
    H2S_cnt++;
    if(H2S_cnt > 4095)
    {
        H2S_cnt = 0;
    }
    CO_cnt++;
    if(CO_cnt > 4095)
    {
        CO_cnt = 0;
    }
    NH3_cnt++;
    if(NH3_cnt > 4095)
    {
        NH3_cnt = 0;
    }
    MCP_DAC_OutVolt(DAC_CH_NO2, O3_cnt);
    MCP_DAC_OutVolt(DAC_CH_CO, H2S_cnt);
    MCP_DAC_OutVolt(DAC_CH_H2S, CO_cnt);
    MCP_DAC_OutVolt(DAC_CH_O3, NH3_cnt);
    
    ADCSensors.O3 = Read_ADS1115(ADC_CH_O3);
    ADCSensors.H2S = Read_ADS1115(ADC_CH_H2S);
    ADCSensors.CO = Read_ADS1115(ADC_CH_CO);
    ADCSensors.NH3 = Read_ADS1115(ADC_CH_NH3);

    Serial.print("O3(");
    Serial.print(ADCSensors.O3);
    Serial.print(")\tH2S(");
    Serial.println(ADCSensors.H2S);
    Serial.print(")\tCO(");
    Serial.print(ADCSensors.CO);
    Serial.print(")\tNH3(");
    Serial.print(ADCSensors.NH3);
    Serial.println(")");
}

