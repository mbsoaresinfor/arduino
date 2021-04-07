#include <ThingSpeak.h>
#include <WiFi.h>

// Network parameters
const char* ssid     = "MBS_EXT";
const char* password = "10607620";

// ThingSpeak information
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long channelID = 1350904;
char* readAPIKey = "C669YRGVI4DF2L03";
char* writeAPIKey = "QD4A1OL80XP6Y9K0";
const unsigned long postingInterval = 120L * 1000L;
unsigned int dataFieldOne = 1;                            // Field to write temperature data
unsigned int dataFieldTwo = 2;                       // Field to write temperature data

unsigned long lastConnectionTime = 0;
long lastUpdateTime = 0; 
WiFiClient client;

int contador = 0;
void setup() {

  Serial.begin(9600);
  Serial.println("Start");
  connectWiFi();
}

void loop() {
    delay(5000);     
//    if (millis() - lastUpdateTime >=  postingInterval) {
//        
//        float fahrenheitTemperature, celsiusTemperature;
//        
//        lastUpdateTime = millis();
//        
//        float readValue = analogRead(A0);
//        
//        Serial.println("temperatura : " + readValue);
//        writeTSData( channelID , dataFieldOne , readValue );    
//    }
  writeTSData( channelID , dataFieldOne , contador++ );  
}

void connectWiFi(){
    
    while (WiFi.status() != WL_CONNECTED) {
      
        WiFi.begin( ssid, password );
        delay(2500);
        Serial.println("Connecting to WiFi");
    }
    
    Serial.println( "Connected" );
    ThingSpeak.begin( client );
}


float readTSData( long TSChannel,unsigned int TSField ){
    
  float data =  ThingSpeak.readFloatField( TSChannel, TSField, readAPIKey );
  Serial.println( " Data read from ThingSpeak: " + String( data, 9 ) );
  return data;

}

// Use this function if you want to write a single field.
int writeTSData( long TSChannel, unsigned int TSField, float data ){
  int  writeSuccess = ThingSpeak.writeField( TSChannel, TSField, data, writeAPIKey ); // Write the data to the channel
  if ( writeSuccess ){
    
    Serial.println( String(data) + " written to Thingspeak. Status: " + writeSuccess );
    }
    
    return writeSuccess;
}

// Use this function if you want to write multiple fields simultaneously.
int write2TSData( long TSChannel, unsigned int TSField1, float field1Data, unsigned int TSField2, long field2Data, unsigned int TSField3, long field3Data ){

  ThingSpeak.setField( TSField1, field1Data );
  ThingSpeak.setField( TSField2, field2Data );
  ThingSpeak.setField( TSField3, field3Data );
   
  int writeSuccess = ThingSpeak.writeFields( TSChannel, writeAPIKey );
  return writeSuccess;
}
