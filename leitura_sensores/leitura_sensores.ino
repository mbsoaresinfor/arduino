#include <OneWire.h> // sensor temperatura
#include <DallasTemperature.h> // sensor temperatura
#include <Servo.h> // servo
#include <LiquidCrystal.h> // liquid cristal.
#include <Rtc_Pcf8563.h> // real time clock

// ativa/desativa debug
const boolean DEBUG_ENABLE = true;

// definicao dos pinos
const int buttonPin = 12;    
const int ledPin =  13;      
const int buzzerPin = 10;
const int sensorTemperaturaPin = 7; // Define pino do sensor
const int servoPin =  6 ; 
const int releUmPin = 5;
int sensorLuzAnalogicoPin = A0; 

// objetos.
OneWire oneWire(sensorTemperaturaPin); // Cria um objeto OneWire
DallasTemperature sensor(&oneWire); // Informa a referencia da biblioteca dallas temperature para Biblioteca onewire
DeviceAddress endereco_temp; // Cria um endereco temporario da leitura do sensor
Servo s; // Variável Servo
LiquidCrystal lcd(2, 3, 4, 6, 8, 9); // definicao das portas do led
Rtc_Pcf8563 rtc; // realtime

// variaveis diversas.
int pos; // Posição Servo  

void setup() {
  Serial.begin(9600);   
  setupPinos();
  setupSensorTemperatura();  
  setupServoMotor();
  setupClock();
}

void loop() {
  mostraData();
  mostraHora();

  processaCliqueBotao(leituraBotao());
  
  float temperatura = leituraSensorTemperatura();  
  int luminosidade = leituraSensorLuz();
  //ligaRelePelaTemperatura(temperatura);  
  //ligaReleNoHorario(7,50);
  //desligaReleNoHorario(8,05);
  delay(1000);

}


void ligaReleNoHorario(int hora, int minuto){
  if( rtc.getHour() == hora && rtc.getMinute() == minuto){
    String message = "Ligando rele ";
    message.concat(String(hora));
    message.concat(String(":"));
    message.concat(String(minuto));
    digitalWrite(releUmPin, LOW,message);
  }
}

void desligaReleNoHorario(int hora, int minuto){
  if( rtc.getHour() == hora && rtc.getMinute() == minuto){
    String message = "Desligando rele ";
    message.concat(String(hora));
    message.concat(String(":"));
    message.concat(String(minuto));
    digitalWrite(releUmPin, HIGH,message);
  }
}

void setupPinos(){
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin,OUTPUT);
  pinMode(releUmPin,OUTPUT);
  digitalWrite(releUmPin, HIGH); 
}


void mostraData(){
   Serial.print("Data Atual: ");
   Serial.println(rtc.formatDate());
}

void mostraHora(){
   Serial.print("Hora Atual: ");
   Serial.println(rtc.formatTime());
}

void digitalWrite(int pin, int status, String texto){
  int statusAtual = digitalRead(pin);
  if(statusAtual != status){
    digitalWrite(pin, status);   
    Serial.println(texto);
  }
}
void  processaCliqueBotao(int buttonState){
  
  if (buttonState == HIGH) {
    
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin,1000);
    delay(500);
    noTone(buzzerPin);    
    
  } else {    
    digitalWrite(ledPin, LOW);    
  }
  
}


int leituraBotao(){
  return digitalRead(buttonPin);  
}

int leituraSensorLuz(){
  int ret =   analogRead(sensorLuzAnalogicoPin);
  String message = "Luminosidade: ";
  message.concat(String(ret));    
  debug(message); 
  return ret;
}

float leituraSensorTemperatura(){
  float temperatura = 0;
  sensor.requestTemperatures(); // Envia comando para realizar a conversão de temperatura
  if (!sensor.getAddress(endereco_temp,0)) { // Encontra o endereco do sensor no barramento
    Serial.println("SENSOR NAO CONECTADO"); // Sensor conectado, imprime mensagem de erro
  } else {    
    temperatura = sensor.getTempC(endereco_temp);
    String message = "Temperatura: ";
    message.concat(String(temperatura));
    message.concat(" C");
    debug(message);  
  }
  return temperatura;
}


void debug(String texto){
  if(DEBUG_ENABLE){
    Serial.print("DEBUG: ");
    Serial.println(texto);
  }
} 

void debug(float texto){
  if(DEBUG_ENABLE){
    Serial.print("DEBUG: ");
    Serial.println(texto);
  }
}


void setupSensorTemperatura(){
  sensor.begin();  // Inicia o sensor temperatura
}

void setupServoMotor(){
  s.attach(servoPin); // inicia servo com sua porta.
  s.write(0); // Inicia motor posição zero
}

void setupClock(){
  // codigo que deve ser executado para setar o modulo realtime
 // rtc.initClock();
  //set a time to start with.
  //day, weekday, month, century, year
//    rtc.setDate(04, 3, 3, 21, 21);
//    //hr, min, sec
//    rtc.setTime(9, 26, 00);
  
}
