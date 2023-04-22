///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// SPS30 ////////////////////////////////////////////////////////////////////
#include <sps30.h>

// SPS30 Init Declaration //
int16_t ret;
uint8_t auto_clean_days = 4;
uint32_t auto_clean;

void sps30_setup() {

  sensirion_i2c_init();

  while (sps30_probe() != 0) {
    Serial.print("SPS sensor probing failed\n");
    // delay(500);
  }

#ifndef PLOTTER_FORMAT
  Serial.print("SPS sensor probing successful\n");
#endif /* PLOTTER_FORMAT */

  ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
  if (ret) {
    Serial.print("error setting the auto-clean interval: ");
    Serial.println(ret);
  }

  ret = sps30_start_measurement();
  if (ret < 0) {
    Serial.print("error starting measurement\n");
  }


#ifdef SPS30_LIMITED_I2C_BUFFER_SIZE
  // Serial.print("Your Arduino hardware has a limitation that only\n");
  // Serial.print("  allows reading the mass concentrations. For more\n");
  // Serial.print("  information, please check\n");
  // Serial.print("  https://github.com/Sensirion/arduino-sps#esp8266-partial-legacy-support\n");
  // Serial.print("\n");
  // delay(2000);
#endif

//  delay(1000);
}

void sps30_loop() 
{
    Serial.println("sps30 Measurement");
    struct sps30_measurement m;
    char serial[SPS30_MAX_SERIAL_LEN];
    uint16_t data_ready;

    do 
    {
        ret = sps30_read_data_ready(&data_ready);
        if (ret < 0) 
        {
            Serial.print("error reading data-ready flag: ");
            Serial.println(ret);
        }
        else if (!data_ready)
        {
//            Serial.print("data not ready, no new measurement available\n");
        }
        else
        {
            break;
        }
    } while (1);

    ret = sps30_read_measurement(&m);
    if (ret < 0) 
    {
        Serial.print("error reading measurement\n");
    }
    else 
    {
#ifndef PLOTTER_FORMAT
        Serial.print("PM  1.0: ");
        Serial.println(m.mc_1p0);
        Serial.print("PM  2.5: ");
        Serial.println(m.mc_2p5);
        Serial.print("PM  4.0: ");
        Serial.println(m.mc_4p0);
        Serial.print("PM 10.0: ");
        Serial.println(m.mc_10p0);

#ifndef SPS30_LIMITED_I2C_BUFFER_SIZE
        Serial.print("NC  0.5: ");
        Serial.println(m.nc_0p5);
        Serial.print("NC  1.0: ");
        Serial.println(m.nc_1p0);
        Serial.print("NC  2.5: ");
        Serial.println(m.nc_2p5);
        Serial.print("NC  4.0: ");
        Serial.println(m.nc_4p0);
        Serial.print("NC 10.0: ");
        Serial.println(m.nc_10p0);
    
        Serial.print("Typical partical size: ");
        Serial.println(m.typical_particle_size);
#endif

        Serial.println();

#else
    // since all values include particles smaller than X, if we want to create buckets we
    // need to subtract the smaller particle count.
    // This will create buckets (all values in micro meters):
    // - particles        <= 0,5
    // - particles > 0.5, <= 1
    // - particles > 1,   <= 2.5
    // - particles > 2.5, <= 4
    // - particles > 4,   <= 10

        Serial.print(m.nc_0p5);
        Serial.print(" ");
        Serial.print(m.nc_1p0  - m.nc_0p5);
        Serial.print(" ");
        Serial.print(m.nc_2p5  - m.nc_1p0);
        Serial.print(" ");
        Serial.print(m.nc_4p0  - m.nc_2p5);
        Serial.print(" ");
        Serial.print(m.nc_10p0 - m.nc_4p0);
        Serial.println();
#endif /* PLOTTER_FORMAT */

  }
}

//////////////////////////////////////////////////// SPS30 ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
