#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
WiFiClient  client;
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Serial.begin(115200);      // Initialize serial 
  dht.begin();
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
      delay(5000);     // Delaying 5 seconds if it is not connected the first line
    } 
    Serial.println("\nConnected.");
  }

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);
  
  // write to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(60000); // Wait 60 seconds to update the channel again
}
