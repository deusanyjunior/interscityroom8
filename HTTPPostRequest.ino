#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <time.h>

/////////////////////////////////////////////////////////////////////////
// Wireless settings
const char* ssid = "wifissid";
const char* password = "wifipassword";

////////////////////////////////////////////////////////////////////////
//InterSCity host
const char* host = "143.107.45.126";
const int hostPort = 30134;

// InterSCity uuid and resource
const char* uuid = "e8dcf7f8-4977-4295-a955-3ee3daf18714";
const char* resource = "room_monitoring";

////////////////////////////////////////////////////////////////////////
// NTP resources

unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "0.br.pool.ntp.org";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

// A time instance
time_t rawTime;
time_t startTime;
time_t now;
struct tm ts;
char timeBuffer[80];
bool timeDefined = false;

///////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);
  Serial.println();

  wifiConnection();

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    // Get current time from NTP
    if (!timeDefined) {
      if (!getTimeNTP()) return; // Get time from NTP servers
      timeDefined = true;
    }
    // set current time based on statup time
    time (&rawTime);
    now = rawTime + startTime; 
    ts = *localtime(&now);
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%dT%H:%M:%S.000Z", &ts);
    
    // Post a message to InterSCity
    Serial.println("\nNew HTTP Post Request to InterSCity Platform");
    
    // define the post URL
    // http://143.107.45.126:30134/adaptor/resources/{uuid}/data/{resource}
    String address = String("http://")
                     + String(host) + ":"
                     + String(hostPort)
                     + "/adaptor/resources/"
                     + String(uuid)
                     + "/data/"
                     + String(resource);
    Serial.print("Host address: ");
    Serial.println(address);
    
    // define JSON content
    // example: {"data":[{"ldr":963,"timestamp":"2018-05-17T15:01:23.000Z"}]}
    char JSONmessageBuffer[300];
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonArray& data = root.createNestedArray("data");
    JsonObject& dataObjects = data.createNestedObject();

    // add light data
    int ldr = analogRead(A0);
    dataObjects["ldr"] = ldr;
    // add timestamp data
    dataObjects["timestamp"] = String(timeBuffer);

    // define post message
    String postMessage;
    root.printTo(postMessage);

    // post message
    HTTPClient http;
    http.begin(address);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(postMessage);  //Send the request
    String payload = http.getString();      //Get the response payload

    Serial.print("JSON message:");
    Serial.println(postMessage); //Print JSON message
    Serial.print("HTTP return code: ");
    Serial.println(httpCode);    //Print HTTP return code
    Serial.print("Payload: ");
    Serial.println(payload);     //Print request response payload
    
    http.end();  //Close connection

  } else {

    Serial.println("Error in WiFi connection. Trying to reconnect..");
    wifiConnection();

  }

  // post every 30 seconds
  delay(30000);
}

////////////////////////////////////////////////////////////////////////////////////
// Useful methods

// Method to (re)connect to WiFi network
void wifiConnection() {

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

// Method to get the NTP response and backup the *startTime*
bool getTimeNTP() {
  Serial.println("Get time from NTP servers");
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no NTP packet yet");
    delay(10000);
    return false;
  }
  else {
    Serial.print("NTP packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
   
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    
    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
   
    // subtract seventy years:
    //unsigned long epoch = secsSince1900 - seventyYears;
    now = secsSince1900 - seventyYears;

    // backup time after seventy years 
    time( &rawTime );
    startTime = now - rawTime;

    Serial.println("Time defined");
    
    return true;
  }
}

// Method to send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
