#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <SoftwareSerial.h>
SoftwareSerial espSerial(D7, D6);//(Rx,TX)
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
String data;
String myStatus = "";
void setup() {
  Serial.begin(9600);
  espSerial.begin(115200);
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

data = espSerial.readStringUntil('\r');
Serial.println(data);
String V1=data.substring(0,4);
String V2=data.substring(7,11);
String V3=data.substring(14,18);
String TX=data.substring(21,26);
String C=data.substring(29,34);
Serial.println(V1);
Serial.println(V2);
Serial.println(V3);
Serial.println(TX);
Serial.println(C);
//int V_1= V1.toInt();
//int V_2= V2.toInt();
//int V_3= V3.toInt();
//int T_X= TX.toInt();
//int Cur= C.toInt();

  // set the fields with the values
  ThingSpeak.setField(1, V1);
  ThingSpeak.setField(2, V2);
  ThingSpeak.setField(3, V3);
  ThingSpeak.setField(4, TX);
  ThingSpeak.setField(5, C);

  
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
   
}
