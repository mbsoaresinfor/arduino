// ******* INFORMACOES
// USAR A PLACA ESP32 DEV MODULE
// E TAMBEM, PODE SER USADO DOIT ESP DEVKIT V1

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

const char* ssid = "MBS_EXT";
const char* password = "10607620";

WebServer server(80);

const int relePin = 23;

void handleRoot() {  
  server.send(200, "text/html", menuMain());  
}

void handleNotFound() {  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/html", message);
  
}

void setup(void) {
  pinMode(relePin, OUTPUT);    
      digitalWrite(relePin, HIGH);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/liga", []() {
    digitalWrite(relePin, LOW);
    server.send(200, "text/html", menuMain());
    
  });

  server.on(UriBraces("/desliga"), []() {        
    digitalWrite(relePin, HIGH);
     server.send(200, "text/html", menuMain());
    
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}

String descricaoStatusRele(){
  int resultado = digitalRead(relePin);
  if(resultado == HIGH){
    return "Desligado";
  }else{
    return "Ligado";
  }
}
String menuMain(){
String page = "<!DOCTYPE html><html><meta charset='UTF-8'><body><h2><center>A lampada está: " +  descricaoStatusRele() + 
      ".</br><a href='/liga\'>Ligar </a> </br>" +
      "<a href='/desliga\'>Desligar </a> </br>" +
      "</h2></center></body></html>";
return page;
  
}
