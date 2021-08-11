#include <OneWire.h>
#include <DallasTemperature.h>

//SENSOR TEMPERATURA AGUA
//OneWire ourWire(4); //Se establece el pin 2 como bus
//OneWire
//DallasTemperature sensors(&ourWire); //Se declara una variable u
//objeto para nuestro sensor

//SENSOR DE FLUJO
const int sensorPin = 2;
const int measureInterval = 100;
volatile int pulseConter;
float fin2=0;
const float factorK = 7.5;

void ISRCountPulse(){
  pulseConter++;
}

float GetFrequency(){
  pulseConter = 0;
  interrupts();
  delay(measureInterval);
  noInterrupts();
  return (float)pulseConter * 1000 / measureInterval;
}

///////////////////////////////////////////////////////////////////////////////////
void setup() {
  //VOID SETUP SENSOR FLUJO
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(sensorPin), ISRCountPulse,RISING);
  
  //VOID SETUP SENSOR TEMP AGUA
  delay(2000);
  Serial.begin(9600);
  //sensors.begin(); //Se inicia el sensor
}

///////////////////////////////////////////////////////////////////////////////////

void loop() {
  //VOID LOOP SENSOR FLUJO
  float frequency = GetFrequency();// obtener frecuencia en Hz
  float flow_Lmin = frequency / factorK;// calcular caudal L/min
  fin2=flow_Lmin;
  
  Serial.println("****************************************************************");
  Serial.print("Frecuencia: ");
  Serial.print(frequency, 0);
  Serial.print(" (Hz)\tCaudal: ");
  Serial.print(flow_Lmin, 3);
  Serial.println(" (L/min)");
  //VOID LOOP SENSOR TEMP AGUA
//  sensors.requestTemperatures(); //Se envía el comando para leer la
//  temperatura
//  float temp= sensors.getTempCByIndex(0); //Se obtiene la temperatura
//  en oC
//  Serial.print("Temperatura= ");
//  Serial.print(temp);
//  Serial.println(" C");
  delay(2000);
}
