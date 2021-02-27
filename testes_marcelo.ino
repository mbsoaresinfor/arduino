#include <OneWire.h> // sensor temperatura
#include <DallasTemperature.h> // sensor temperatura
#include <Servo.h> // servo
#include <LiquidCrystal.h> // liquid cristal.

// ativa/desativa debug
const boolean DEBUG_ENABLE = true;

// definicao dos pinos
const int buttonPin = 12;    
const int ledPin =  13;      
const int buzzerPin = 10;
const int sensorTemperaturaPin = 7; // Define pino do sensor
const int servoPin =  6 ; 
const int releUmPin = 5;
//const int realTimeclock = 5;


// objetos.
OneWire oneWire(sensorTemperaturaPin); // Cria um objeto OneWire
DallasTemperature sensor(&oneWire); // Informa a referencia da biblioteca dallas temperature para Biblioteca onewire
DeviceAddress endereco_temp; // Cria um endereco temporario da leitura do sensor
//Rtc_Pcf8563 rtc; //init the real-time clock
Servo s; // Variável Servo
LiquidCrystal lcd(2, 3, 4, 6, 8, 9); // definicao das portas do led

// variaveis diversas.
int pos; // Posição Servo  



void setup() {
  Serial.begin(9600);   
  setupPinos();
  setupSensorTemperatura();  
  setupServoMotor();
 // setupClock();

}

void loop() {

  processaCliqueBotao(leituraBotao());

  float temperatura = leiturasensorTemperaturaPin();  
  if(temperatura > 26){
   digitalWrite(releUmPin, LOW,"Ligando rele"); 
  }else{
    digitalWrite(releUmPin, HIGH,"Desligando rele"); 
  }

}


void setupPinos(){
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin,OUTPUT);
  pinMode(releUmPin,OUTPUT);
  //pinMode(realTimeclock, INPUT); 
  digitalWrite(releUmPin, HIGH); 
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

float leiturasensorTemperaturaPin(){
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
