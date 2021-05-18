#include <ThingSpeak.h>
#include <WiFi.h>
#include <OneWire.h> // sensor temperatura
#include <DallasTemperature.h> // sensor temperatura
#include <HTTPClient.h>

const int sensorTemperaturaPin = 22;
const int relePin = 23;

// objetos 
OneWire oneWire(sensorTemperaturaPin); // Cria um objeto OneWire
DallasTemperature sensor(&oneWire); // Informa a referencia da biblioteca dallas temperature para Biblioteca onewire
DeviceAddress endereco_temp; // Cria um endereco temporario da leitura do sensor

// Network parameters
const char* ssid     = "MBS_EXT";
const char* password = "10607620";

unsigned long lastConnectionTime = 0;
long lastUpdateTime = 0; 
WiFiClient client;

const float temperaturaIdeal = 20;

String endpoint_update = "http://192.168.0.12:8080/update";

void setup() {

  Serial.begin(9600);
  Serial.println("Start");
  pinMode(relePin, OUTPUT);
  connectWiFi();
}

void loop() {

   Serial.print("Temperatura Ideal => " );  
   Serial.println( temperaturaIdeal );  
  float temperature = leituraSensorTemperatura();
  if(temperature < temperaturaIdeal){
    digitalWrite(relePin, HIGH, "LIGANDO LAMPADA");         
  }else{
    digitalWrite(relePin, LOW, "DESLIGANDO LAMPADA");             
  }

  leituraRele(relePin);

  if(WiFi.status()== WL_CONNECTED){
     Serial.println("WiFi Connected");
      HTTPClient http;

      String serverPath = endpoint_update + "?field=1&value=" + temperature;

      Serial.println("Connecting " + serverPath);
      
      http.begin(serverPath.c_str());
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    delay(5000);     
  
}

void connectWiFi(){
    
    while (WiFi.status() != WL_CONNECTED) {
      
        WiFi.begin( ssid, password );
        delay(2500);
        Serial.println("Connecting to WiFi");
    }
    
    Serial.println( "Connected" );
 
}

void digitalWrite(int pin, int status, String texto){
  int statusAtual = digitalRead(pin);
  if(statusAtual != status){
    digitalWrite(pin, status);   
    Serial.println(texto);
  }
}

void leituraRele(int pin){
  int statusAtual = digitalRead(pin);  
  if(statusAtual == HIGH){    
    Serial.println("LAMPADA ESTA LIGADA");
  }else{
    Serial.println("LAMPADA ESTA DESLIGADA");
  }
}

float leituraSensorTemperatura(){
  float temperatura = 0;
  sensor.requestTemperatures(); // Envia comando para realizar a conversão de temperatura
  if (!sensor.getAddress(endereco_temp,0)) { // Encontra o endereco do sensor no barramento
    Serial.println("SENSOR TEMPERATURA NAO CONECTADO"); // Sensor conectado, imprime mensagem de erro    
  } else {    
    temperatura = sensor.getTempC(endereco_temp);
    String message = "Temperatura Atual = ";
    message.concat(String(temperatura));
    message.concat(" C");
    Serial.println(message);  
  }
  return temperatura;
}
