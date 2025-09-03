#include <Wire.h>
//#include "SparkFunMPL3115A2.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

#include "baro.h"
#include "vfb.h"

#define SEALEVELPRESSURE_HPA (1013.25)

void set_sealevel(float pressure);
void I2C_Write(byte regAddr, byte value);

//Create an instance of the object
//MPL3115A2 baro;
Adafruit_BMP3XX baro;

void baro_init(void) {
  Serial.println("misc::BMP390::configuration...");
  if (!baro.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  //if (! baro.begin_SPI(baro_CS)) {  // hardware SPI mode  
  //if (! baro.begin_SPI(baro_CS, baro_SCK, baro_MISO, baro_MOSI)) {  // software SPI mode
    Serial.println("misc::BMP390::Could not find a valid baro3 sensor, check wiring!");
    //while (1);
  } else {
    // Set up oversampling and filter initialization
    baro.setPressureOversampling( BMP3_OVERSAMPLING_16X);
    baro.setTemperatureOversampling( BMP3_OVERSAMPLING_2X);
    baro.setIIRFilterCoeff( BMP3_IIR_FILTER_COEFF_7);
    baro.setOutputDataRate( BMP3_ODR_100_HZ);
	  baro.performConfig( BMP3_MODE_NORMAL);
  }
  internals.bmp390_rd_fail_cnt = 0;
  Serial.println( "misc::BMP390::I2C.");
}

void baro_step( float* pressure, float* degC) {
	float p,t;
  // NOTE: readPandT --> this is a custom function to optimize i2c bandwidth - mvs
	bool rd_ok = baro.readPandT( &p, &t);
	*pressure = p;
	*degC = t;
  if( !rd_ok) {
    internals.bmp390_rd_fail_cnt++;
  }
}

void get_baro_temp( float* degC) {
  *degC = baro.readTemperature();
}

/*
void set_sealevel(float pressure) {
  uint16_t P_value = pressure*500;
  uint8_t register_list[2] = {P_value>>8, P_value};
  //reg_w( MPL3115_BAR_IN_MSB, register_list, 2);
  I2C_Write( BAR_IN_MSB, register_list[0]);
  I2C_Write( BAR_IN_LSB, register_list[1]);
}

void I2C_Write(byte regAddr, byte value)
{
  // This function writes one byto over IIC
  Wire.beginTransmission(0x60);
  Wire.write(regAddr);
  Wire.write(value);
  Wire.endTransmission(true);
}
*/
 //References: 
  //Definition of "altimeter setting": http://www.crh.noaa.gov/bou/awebphp/definitions_pressure.php
  //Altimeter setting: http://www.srh.noaa.gov/epz/?n=wxcalc_altimetersetting
  //Altimeter setting: http://www.srh.noaa.gov/images/epz/wxcalc/altimeterSetting.pdf
  //Verified against Boulder, CO readings: http://www.crh.noaa.gov/bou/include/webpres.php?product=webpres.txt
