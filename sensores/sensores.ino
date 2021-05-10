//LIBRERIAS ///////////////////////////////////////////////////////////////////////////////////////////////////

#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>//LIBRERIA DEL SENSOR DE HUMEDAD Y TEMPERATURA
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

//PUERTOS /////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //Puerto sensor de flujo
  const int sensorPin = 7;
  
  //Puertos sensor nivel de agua
  #define pulso 3  //define la salida por donde se manda el pulso como 9
  #define rebote 4 //define la salida por donde se recibe el rebote como 10
  
  //Puertos sensor de humedad y temperatura
  #define DHTPIN 2//define la salida por donde se recibe el sensor DHT como 10
  #define DHTTYPE DHT11 //se define el tipo de sensor, el cual es 11

  //Puerto sensor de lluvia
  #define in_lluvia 5//se define el tipo de sensor
  #define analog_lluvia A0
  
//DECLARACIONES ///////////////////////////////////////////////////////////////////////////////////////////////

  //Declaración para el sensor de temperatura y humedad
  DHT dht(DHTPIN, DHTTYPE);

  //Declaración de la pantalla
  LiquidCrystal lcd(23, 25, 27, 29, 31, 33);

//VARIABLES ///////////////////////////////////////////////////////////////////////////////////////////////////
  
  //Variables para el sensor de flujo
  const int measureInterval = 100;
  volatile int pulseConter;
  const float factorK = 7.5;
  float fin2=0;
  float frequency = 0.0;
  float flow_Lmin = 0.0;

  //Variable para el sensor de nivel de agua
  int distancia;  //crea la variable "distancia"
  float tiempo;  //crea la variable tiempo (como float)

  //Variable para el sensor de lluvia
  int lluvia; //crea la variable "lluvia"
  int intensidadLluvia = 0;
  boolean estadoLluvia = false;

  //Variable para el sensor de temperatura y humedad ambiental
  int dht_in;//crea la variable "dht"
  int temperaturaAmbiental = 0;
  int humedadAmbiental = 0;

  //Variable pantalla
  int showMessage = 0;  //crea la cariable tecla

//FUNCIONES ///////////////////////////////////////////////////////////////////////////////////////////////////
  
  //Función de pulsos para el sensor de flujo
  void ISRCountPulse(){
    pulseConter++;
  }
  
  //Función para obtener la frecuencia del sensor de flujo
  float GetFrequency(){
    pulseConter = 0;
    delay(measureInterval);
    return (float)pulseConter * 1000 / measureInterval;
  }

  //Función pantalla
  void lcdScreen(){   //funcion para el LCD
   
    if(showMessage == 0){                                 //INICIO
      
      lcd.setCursor(0,0);
      lcd.print("ResCity                      ");
      lcd.setCursor(0,1);
      lcd.print("                             ");
      Serial.println(showMessage);
        
    }else if(showMessage == 1){                         //Frecuencia flujo
    
      lcd.setCursor(0,0);
      lcd.print("Frec Flujo:                  ");
      lcd.setCursor(0,1);
      lcd.print(frequency);
      lcd.print(" (Hz)/caudal                 ");
      Serial.println(showMessage);
        
    }else if(showMessage == 2){                         //Flujo agua
      
      lcd.setCursor(0,0);
      lcd.print("Flujo Agua:                  ");
      lcd.setCursor(0,1);
      lcd.print(flow_Lmin);
      lcd.print(" (L/min)                     ");
      Serial.println(showMessage);
      
    }else if(showMessage == 3){                         //Nivel agua
         
      lcd.setCursor(0,0);
      lcd.print("Nivel Agua:                  ");
      lcd.setCursor(0,1);
      lcd.print(distancia);
      lcd.print(" cm                          ");
      Serial.println(showMessage);
      
    }else if(showMessage == 4){                         //Intensidad lluvia
      
      lcd.setCursor(0,0);
      lcd.print("Int Lluvia:                  ");
      lcd.setCursor(0,1);
      lcd.print(intensidadLluvia);
      lcd.print("                             ");
      Serial.println(showMessage);
      
    }else if(showMessage == 5){                         //Estado lluvia
      
      lcd.setCursor(0,0);
      lcd.print("Estado Lluvia:               ");
      lcd.setCursor(0,1);
      lcd.print(estadoLluvia);
      lcd.print("                             ");
      Serial.println(showMessage);
      
    }else if(showMessage == 6){                         //Humedad ambiental
      
      lcd.setCursor(0,0);
      lcd.print("Humedad Amb:                 ");
      lcd.setCursor(0,1);
      lcd.print(humedadAmbiental);
      lcd.print(" %                           ");
      Serial.println(showMessage);
      
    }else if(showMessage == 7){                         //Temperarua ambiental
      
      lcd.setCursor(0,0);
      lcd.print("Temperatura Amb:              ");
      lcd.setCursor(0,1);
      lcd.print(temperaturaAmbiental);
      lcd.print(" *C                          ");
      Serial.println(showMessage);
      
    }    
  }

void setup()
{
  Serial.begin(9600);  //inicializa el puerto serie

  //Inicialización para el sensor de flujo
  attachInterrupt(digitalPinToInterrupt(sensorPin), ISRCountPulse,RISING); 

  //Inicialización para el sensor de nivel de agua
  pinMode(pulso, OUTPUT); //Declaramos el pin 3 como salida (pulso ultrasonido)
  pinMode(rebote, INPUT); //Declaramos el pin 4 como entrada (recepción del pulso)

  //Inicialización para el sensor de lluvia
  pinMode(lluvia, INPUT); //Declaramos el pin 5 como entrada
  
  //Inicialización para el sensor de temperatura y humedad ambiental
  dht.begin();

  //Inicialización pantalla
  lcd.begin(16,2);
  
}
 
void loop()
{
  // INCREMNTADOR PANTALLA /////////////////////////////////////////////////////////////////////////////////////
    
    showMessage++;

    if(showMessage > 7){       //SUMADOR CON REINICIO LCD
      showMessage=0;
    }

  //VOID LOOPS /////////////////////////////////////////////////////////////////////////////////////////////////
  
    //VOID LOOP sensor de flujo
    frequency = GetFrequency();// obtener frecuencia en Hz
    flow_Lmin = frequency / factorK;// calcular caudal L/min
    fin2=flow_Lmin;
  
    //VOID LOOP para el sensor de nivel de agua
    digitalWrite(pulso,LOW); //Por cuestión de estabilización del sensor
    delayMicroseconds(5);
    digitalWrite(pulso, HIGH); //envío del pulso ultrasónico
    delayMicroseconds(10);
    tiempo = pulseIn(rebote, HIGH);  //funcion para medir el tiempo y guardarla en la variable "tiempo"
    distancia = 0.01715*tiempo; //fórmula para calcular la distancia

    //VOID LOOP sensor de lluvia
    intensidadLluvia = analogRead(analog_lluvia); 
    lluvia=digitalRead(in_lluvia); //lectura del sensor de lluvia
    if(lluvia==LOW){
      estadoLluvia = true;
    }else{
      estadoLluvia = false;
    }

    //VOID LOOP para el sensor de temperatura y humedad ambiental
    temperaturaAmbiental = dht.readTemperature();  //TOMA DE DATOS TEMPERATURA SENSOR DTH
    humedadAmbiental = dht.readHumidity();     //TOMA DE DATOS HUMEDAD SENSOR DTH

   //SERIAL ////////////////////////////////////////////////////////////////////////////////////////////////////

    Serial.println("SALIDA SENSORES ***************************************");
    //Serial sensor de flujo
    Serial.print("Frecuencia: ");
    Serial.print(frequency, 0);
    Serial.print(" (Hz)\tCaudal: ");
    Serial.print(flow_Lmin, 3);
    Serial.println(" (L/min)");

    //Serial sensor de nivel de agua
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");

    //Serial sensor de lluvia
    Serial.print("Intensidad lluvia: ");
    Serial.print(intensidadLluvia);
    Serial.print(" - ");
    Serial.print(" Estado lluvia: ");
    Serial.print(estadoLluvia);
    Serial.println(" ");

    //Serial sensor de temperatura y humeda ambiental
    Serial.print("Humedad relativa: ");
    Serial.print(humedadAmbiental);
    Serial.print(" %\t");
    Serial.print("Temperatura: ");
    Serial.print(temperaturaAmbiental);
    Serial.println(" *C");

    Serial.println("*******************************************************");

  //ENVIO DE DATOS AL ESP32 ////////////////////////////////////////////////////////////////////////////////////

     //Declaración DOC para el JSON
    StaticJsonDocument<256> doc;
    
    //Creació de un JSON Vacio
    JsonObject json = doc.to<JsonObject>();
  
    //Agregar las claves y valores de las variables que se quiere mandar al esp32
    json["frecuancia_flujo"] = String(frequency);
    json["flujo_agua"] = String(flow_Lmin);
    json["nivel_agua"] = String(distancia);
    json["intensidad_lluvia"] = String(intensidadLluvia);
    json["estado_lluvia"] = String(estadoLluvia);
    json["huemdad_ambiental"] = String(humedadAmbiental);
    json["temperatura_ambiental"] = String(temperaturaAmbiental);

    //Serializción JSON
    String output;
    serializeJsonPretty(json, output);

    //Poner JSON en el serial 9600 
    Serial.println("SALIDA JSON ----------------------------------------------");
    Serial.println(output);
    Serial.println("----------------------------------------------------------");

  //MOSTRAR EN PANTALLA /////////////////////////////////////////////////////////////////////////////////////////
    
    lcdScreen();
    
  delay(2000); //TIEMPO DEL PROCESO
}
