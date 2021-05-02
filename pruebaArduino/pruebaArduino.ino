#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define pulso 9  //define la salida por donde se manda el pulso como 9
#define rebote 8 //define la salida por donde se recibe el rebote como 10
 
int distancia;  //crea la variable "distancia"
float tiempo;  //crea la variable tiempo (como float)
String str;

String message = "";
bool messageReady = false;

//SoftwareSerial espSerial(17,16); //RX TX
 
void setup()
{
  Serial.begin(115200);  //inicializa el puerto serie
  //espSerial.begin(4800);
  pinMode(pulso, OUTPUT); //Declaramos el pin 9 como salida (pulso ultrasonido)
  pinMode(rebote, INPUT); //Declaramos el pin 8 como entrada (recepción del pulso)
}
 
void loop(){
  
  digitalWrite(pulso,LOW); //Por cuestión de estabilización del sensor
  delayMicroseconds(5);
  digitalWrite(pulso, HIGH); //envío del pulso ultrasónico
  delayMicroseconds(10);
  tiempo = pulseIn(rebote, HIGH);  //funcion para medir el tiempo y guardarla en la variable "tiempo"
  distancia = 0.01715*tiempo; //fórmula para calcular la distancia

  //Enviar datos al ESP32
  StaticJsonDocument<1000> doc;
  JsonObject json = doc.to<JsonObject>();

  json["nivel_agua"] = String(distancia);

  String output;
  serializeJson(json, Serial);
  
  delay(5000);
}
 
