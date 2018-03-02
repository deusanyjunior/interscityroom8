/*
 Based on:
   Arduino example AnalogOutSerial.ino
   Lux sensor: https://github.com/claws/BH1750
   DHT 22: http://www.ardumotive.com/how-to-use-dht-22-sensor-en.html
   ADS1115: http://henrysbench.capnfatz.com/henrys-bench/arduino-voltage-measurements/arduino-ads1115-module-getting-started-tutorial/
*/

#include <Wire.h>
#include <BH1750.h>
#include <dht.h>
#include <Adafruit_ADS1015.h>


// LDR
const int analogInPin = A6;  // Analog input pin that the LDR is attached to
int sensorValueLDR = 0;        // value read from the LDR

// Lux
// BH1750 lightMeter;
uint16_t lux = 0;

// DHT 22
dht DHT;
#define DHT22_PIN 4
float hum = 0;  //Stores humidity value
float temp = 0; //Stores temperature value

// ADS
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
 
 
 // Lux
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  // On esp8266 devices you can select SCL and SDA pins using Wire.begin(D4, D3);
  // Wire.begin();

  //lightMeter.begin();
  
// ADS
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  ads.begin();

// OUTPUT
  Serial.println("sensorLDR,sensorLux,sensorDHT22Humidity,sensorDHT22TemperatureCelsius,sensorADS_0_1,sensorADS_2_3,");
}

void loop() {

// LDR
  // read the value from LDR sensor at A0:
  sensorValueLDR = 1024 - analogRead(analogInPin);     
//  Serial.print("sensorLDR,");
  Serial.print(sensorValueLDR);  
  Serial.print(",");
  
// Lux
  // read the value from Lux sensor:
  //lux = lightMeter.readLightLevel();
//  Serial.print("sensorLux,");
  Serial.print(lux);
  Serial.print(",");
 
 // DHT
  int chk = DHT.read22(DHT22_PIN);
  //Read data and store it to variables hum and temp
  hum = DHT.humidity;
  temp = DHT.temperature;
  //Print temp and humidity values to serial monitor
//  Serial.print("sensorDHT22Humidity,");
    Serial.print(hum);
    Serial.print(",");
//  Serial.print("sensorDHT22TemperatureCelsius,");
    Serial.print(temp);
    Serial.print(",");
   
// ADS
  int16_t adc0, adc1, adc2, adc3;
  // single ended
//  adc0 = ads.readADC_SingleEnded(0);
//  adc1 = ads.readADC_SingleEnded(1);
//  adc2 = ads.readADC_SingleEnded(2);
//  adc3 = ads.readADC_SingleEnded(3);
//  Serial.print(adc0);
//  Serial.print(","); 
//  Serial.print(adc1);
//  Serial.print(","); 
//  Serial.print(adc2);
//  Serial.print(","); 
//  Serial.print(adc3);
//  Serial.print(","); 
  
  int16_t adc_0_1, adc_2_3;
  adc_0_1 = ads.readADC_Differential_0_1();
  adc_2_3 = ads.readADC_Differential_2_3();
  Serial.print(adc_0_1);
  Serial.print(","); 
  Serial.print(adc_2_3);
  Serial.print(","); 
  
   
  Serial.println("");
  delay(1000);  // debug only
//  delay(30000); 
}
