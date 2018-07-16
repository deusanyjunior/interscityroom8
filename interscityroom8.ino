/*
 Based on:
   Arduino example AnalogOutSerial.ino
   Lux sensor: https://github.com/claws/BH1750
   DHT 22: http://www.ardumotive.com/how-to-use-dht-22-sensor-en.html
   ADS1115: http://henrysbench.capnfatz.com/henrys-bench/arduino-voltage-measurements/arduino-ads1115-module-getting-started-tutorial/
*/

#include <Wire.h>
#include <dht.h>
#include <Adafruit_ADS1015.h>

#define __DEBUG__ false

// LDR
const int ldrPin = A7;  // Analog input pin that the LDR is attached to
int sensorValueLDR = 0;        // value read from the LDR

// Soil moisture
const int moisturePin = A6;
int sensorValueMoisture = 0;

// DHT 22
dht DHT;
#define DHT22_PIN 3
float hum = 0;  //Stores humidity value
float temp = 0; //Stores temperature value

// ADS
adsGain_t selectedAdsGain;
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
Adafruit_ADS1115 ads2(0x49);  /* Use this for the 16-bit version */
//int16_t adc0, adc1, adc2, adc3;
int16_t adc_0_1, adc_2_3;
int16_t adc2_0_1, adc2_2_3;

const float voltage_to_temp = 0.039;
const float ads_to_voltage  = 0.0078125;
float magical = ads_to_voltage / voltage_to_temp;
float adcm_0_1, adcm_2_3;

// Print separator
char *sep = ",";

// Heating pulse monitor
int counter = 0;

void sendPulse(int pin, int len) {
  digitalWrite(pin, HIGH);
  delay(len);
  digitalWrite(pin, LOW);

  Serial.print("Sending pulse to pin ");
  Serial.print(pin);
  Serial.print(" with duration = ");
  Serial.print(len);
  Serial.println("ms");
}

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
// LDR
  pinMode (ldrPin, INPUT);
  
// Moisture
  pinMode (moisturePin, INPUT);
 
// ADS
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectlanalogIny may destroy your ADC!
  //                                                                ADS1015  ADS1115            value
  //                                                                -------  -------            -----
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default) 0
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV            512
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV           1024
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV          1536
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV         2048
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV        2560
  
//  selectedAdsGain = GAIN_TWOTHIRDS;
//  selectedAdsGain = GAIN_ONE;
//  selectedAdsGain = GAIN_TWO;
//  selectedAdsGain = GAIN_FOUR;
  selectedAdsGain = GAIN_EIGHT;
//  selectedAdsGain = GAIN_SIXTEEN;
  
  ads.setGain(selectedAdsGain);
  ads2.setGain(selectedAdsGain);
  
  ads.begin();
  ads2.begin();

// Heating
  pinMode(2, OUTPUT); // heating pulse
  digitalWrite(2, LOW);
  //sendPulse(7, 1000);

// OUTPUT
  //Serial.println("sensorLDR,sensorLux,sensorDHT22Humidity,sensorDHT22TemperatureCelsius,sensorADS_0_1,sensorADS_2_3,");
}

void loop() {

// LDR
  // read the value from LDR sensor at ldr:
//  sensorValueLDR = 1024 - analogRead(ldrPin);
  sensorValueLDR = analogRead(ldrPin);

// Moisture
  sensorValueMoisture = analogRead(moisturePin);
  
 // DHT
  int chk = DHT.read22(DHT22_PIN); // ?!?!?!
//  DHT22_ERROR_t errorCode;  
//  errorCode = dht.readData(); // read data from sensor
//
//  if (errorCode == DHT_ERROR_NONE) { // data is good  
//  }  
  //Read data and store it to variables hum and temp
  hum = DHT.humidity;
  temp = DHT.temperature;

// ADS
  // single ended
//  adc0 = ads.readADC_SingleEnded(0);
//  adc1 = ads.readADC_SingleEnded(1);
//  adc2 = ads.readADC_SingleEnded(2);
//  adc3 = ads.readADC_SingleEnded(3);
  // differential  
  adc_0_1 = ads.readADC_Differential_0_1();
  adc_2_3 = ads.readADC_Differential_2_3();
  
  // magical 
  adcm_0_1 = float(adc_0_1) * magical;
  adcm_2_3 = float(adc_2_3) * magical;

// ADS 2
  // differential
  adc2_0_1 = ads2.readADC_Differential_0_1();
  adc2_2_3 = ads2.readADC_Differential_2_3();

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  // OUTPUT 
  
  // debug only
  if (!__DEBUG__) {
    if (counter == 0) digitalWrite(2, HIGH);
    if (counter == 8) digitalWrite(2,  LOW);
  }

// Counter
  Serial.print(counter);
  Serial.print(sep);

// LDR
  //Serial.print("sensorLDR,");
  Serial.print(sensorValueLDR);  
  Serial.print(sep);

// Moisture
  //Serial.print("sensorMoisture,");
  Serial.print(sensorValueMoisture);  
  Serial.print(sep);
 
// DHT
    //Serial.print("sensorDHT22Humidity,");
    Serial.print(hum);
    Serial.print(sep);
    //Serial.print("sensorDHT22TemperatureCelsius,");
    Serial.print(temp);
    Serial.print(sep);
//    Serial.print("\t");
   
// ADS
  // single ended
//  Serial.print(adc0);
//  Serial.print(","); 
//  Serial.print(adc1);
//  Serial.print(","); 
//  Serial.print(adc2);
//  Serial.print(","); 
//  Serial.print(adc3);
//  Serial.print(","); 
  //differential
  Serial.print(adc_0_1);
  Serial.print(sep); 
  Serial.print(adc_2_3);
  Serial.print(sep);
  // magical
  Serial.print(adcm_0_1);
  Serial.print(sep); 
  Serial.print(adcm_2_3);
  Serial.print(sep);
  
// ADS 2
  Serial.print(adc2_0_1);
  Serial.print(sep); 
  Serial.print(adc2_2_3);
  Serial.print(sep);

// ADS GAIN
  Serial.print(selectedAdsGain);
  Serial.print(sep);

// Heating
  Serial.print(digitalRead(2));
  Serial.print(sep);

  Serial.println("");
  //delay(10000);
  if (__DEBUG__) {
    long val = Serial.parseInt();
    if (val == 2)
      digitalWrite(2, HIGH);
    else if (val == 1)
      digitalWrite(2, LOW);
  } else delay(5000);

  counter = (counter + 1) % 32;
}
