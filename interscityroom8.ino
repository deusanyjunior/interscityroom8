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
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
Adafruit_ADS1115 ads2(0x49);  /* Use this for the 16-bit version */
//int16_t adc0, adc1, adc2, adc3;
int16_t adc_0_1, adc_2_3;
int16_t adc2_0_1, adc2_2_3;

const float voltage_to_temp = 0.039;
const float ads_to_voltage  = 0.0078125;
float magical = ads_to_voltage / voltage_to_temp;/*
2
 Based on:
3
   Arduino example AnalogOutSerial.ino
4
   Lux sensor: https://github.com/claws/BH1750
5
   DHT 22: http://www.ardumotive.com/how-to-use-dht-22-sensor-en.html
6
   ADS1115: http://henrysbench.capnfatz.com/henrys-bench/arduino-voltage-measurements/arduino-ads1115-module-getting-started-tutorial/
7
*/
8
​
9
#include <Wire.h>
10
#include <BH1750.h>
11
#include <dht.h>
12
#include <Adafruit_ADS1015.h>
13
​
14
​
15
// LDR
16
const int analogInPin = A6;  // Analog input pin that the LDR is attached to
17
int sensorValueLDR = 0;        // value read from the LDR
18
​
19
// Lux
20
// BH1750 lightMeter;
21
uint16_t lux = 0;
22
​
23
// DHT 22
24
dht DHT;
25
#define DHT22_PIN 4
26
float hum = 0;  //Stores humidity value
27
float temp = 0; //Stores temperature value
28
​
29
// ADS
30
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
31
​
32
void setup() {
33
  // initialize serial communications at 9600 bps:
34
  Serial.begin(9600);
35
 
36
 
37
 // Lux
38
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
39
  // On esp8266 devices you can select SCL and SDA pins using Wire.begin(D4, D3);
40
  // Wire.begin();
41
​
42
  //lightMeter.begin();
43
  
44
// ADS
45
  // The ADC input range (or gain) can be changed via the following
46
  // functions, but be careful never to exceed VDD +0.3V max, or to
47
  // exceed the upper and lower limits if you adjust the input range!
48
  // Setting these values incorrectly may destroy your ADC!
49
  //                                                                ADS1015  ADS1115
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
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
   ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
//  ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  ads2.setGain(GAIN_FOUR);
  
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
  // Prints
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
    Serial.print("\t");
   
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
