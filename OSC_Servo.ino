#define DEBUG_LEVEL 1

#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>
#include <OSCData.h>
#include <OSCMatch.h>
#include <OSCTiming.h>

#include <FS.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ArduinoJson.h>  //Maximum version is with this code is 5.13 see https://articles.aquaiver.com/esp8266-nodemcu-arduino-ide-dynamicjsonbuffer-was-not-declared-in-this-scope/
                          //https://github.com/bblanchon/ArduinoJson

#include <Stepper.h>
#include <Servo.h>

#include <WiFiUdp.h>

WiFiUDP Udp;

//needed for library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          // https://github.com/kentaylor/WiFiManager

#include <DoubleResetDetector.h>  //https://github.com/datacute/DoubleResetDetector

// Number of seconds after reset during which a 
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 10

// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0

DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

const int recv_port = 10000;
const int send_port = 12000;

const char* CONFIG_FILE = "/config.json";

// Variables
Servo myServo[8];
int ServoPins[] = {D1,D2,D3,D4,D5,D6,D7,D8};

// Indicates whether ESP has WiFi credentials saved from previous session
bool initialConfig = false;

// Default configuration values, will be overwritten by WifiPortal()
//StepA1
  char strRemoteIP [30] = "";
  byte RemoteIP[4];
  char OscIndexPos = 0;
//!StepA1

// Function Prototypes

bool readConfigFile();
bool writeConfigFile();

void buildOscAddresses()
{
    for(byte car=0;car<30;car++)
    {
      if (strRemoteIP[car] == '*') OscIndexPos = car;
    }
}

// Setup function
void setup()
{
  // Put your setup code here, to run once
  Serial.begin(115200);
  Serial.println("\n Starting");

  // Initialize the LED digital pin as an output.
  //pinMode(PIN_LED, OUTPUT);
  // Initialize trigger pin
  //pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // Mount the filesystem
  bool result = SPIFFS.begin();
  if (DEBUG_LEVEL >=1 ) Serial.println("SPIFFS opened: " + result);
  if (!readConfigFile())
  {
    Serial.println("Failed to read configuration file, using default values");
  }

  //WiFi.printDiag(Serial); //Uncomment this line if you want to see WiFi password printed
  if (drd.detectDoubleReset())
  {
    Serial.println("Double Reset Detected");
    initialConfig = true;
  }
  if (WiFi.SSID() == "")
  {
    Serial.println("We haven't got any access point credentials, so get them now");
    initialConfig = true;
  }
  if (initialConfig)
  {
    WiFi.mode(WIFI_STA); // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.
    unsigned long startedAt = millis();
    if (DEBUG_LEVEL >=1 ) Serial.print("After waiting ");
    int connRes = WiFi.waitForConnectResult();
    float waited = (millis()- startedAt);
    if (DEBUG_LEVEL >=1 ) Serial.print(waited/1000);
    if (DEBUG_LEVEL >=1 ) Serial.print(" secs in setup() connection result is ");
    if (DEBUG_LEVEL >=1 ) Serial.println(connRes);
  }
  
  if (WiFi.status()!=WL_CONNECTED)
  {
    Serial.println("Failed to connect, finishing setup anyway");
  }
  else
  {
    Serial.print("Local ip: ");
    Serial.println(WiFi.localIP());
  }
  Udp.begin(recv_port);

  for(int i=0;i<8;i++)
  {
    myServo[i].attach(ServoPins[i]);
    myServo[i].write(0);
  }
  delay(1000);
  for(int i=0;i<8;i++) myServo[i].write(100);  
  Serial.println("Servo wiring:");
  Serial.println("-is always black (except if brown)");
  Serial.println("+ is always red");
  Serial.println("Signal can be white orange yellow blue black(rare)");
  Serial.println("The cake is a lie");
  Serial.print("OscIndex=");
  buildOscAddresses();
  Serial.print(OscIndexPos);
}  
