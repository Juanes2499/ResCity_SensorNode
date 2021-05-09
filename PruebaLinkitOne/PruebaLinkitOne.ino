#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <ArduinoJson.h>


#define WIFI_AP "402C"
#define WIFI_PASSWORD "nichoy99082411425"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.

//Indicadores
const int greenPIN = 4;
const int redPIN = 3;
const int bluePIN = 2;

//Server information
#define URL    "http://192.168.0.17"

String id_nodo_sensor = "901ee534-4ca1-4932-a4d8-ef5c4e64dba1";
const char* token_dispositivo = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZF9kaXNwb3NpdGl2byI6IjkwMWVlNTM0LTRjYTEtNDkzMi1hNGQ4LWVmNWM0ZTY0ZGJhMSIsIm1hcmNhIjoiVGVzdCIsInJlZmVyZW5jaWEiOiJUZXN0IHJlZiIsImxhdGl0dWQiOiIxMTExLjk5OSIsImxvbmdpdHVkIjoiLTEuODk4NyIsIm5vbWJyZV9taWNyb3NlcnZpY2lvIjoiU0VOU09SRVNfTlMiLCJlbWFpbF9yZXNwb25zYWJsZSI6Imp1YW4ubmljaG95QHVhby5lZHUuY28iLCJpYXQiOjE2MTg5NTUwNTl9.NprlCdYG18Of5pUVWOKpBnXtPu5s3kSZZ5Bb7g2nVuo";
const char* token_autenticacion = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJJRF9ESVNQT1NJVElWTyI6IjkwMWVlNTM0LTRjYTEtNDkzMi1hNGQ4LWVmNWM0ZTY0ZGJhMSIsIlRPS0VOIjoiZXlKaGJHY2lPaUpJVXpJMU5pSXNJblI1Y0NJNklrcFhWQ0o5LmV5SnBaRjlrYVhOd2IzTnBkR2wyYnlJNklqa3dNV1ZsTlRNMExUUmpZVEV0TkRrek1pMWhOR1E0TFdWbU5XTTBaVFkwWkdKaE1TSXNJbTFoY21OaElqb2lWR1Z6ZENJc0luSmxabVZ5Wlc1amFXRWlPaUpVWlhOMElISmxaaUlzSW14aGRHbDBkV1FpT2lJeE1URXhMams1T1NJc0lteHZibWRwZEhWa0lqb2lMVEV1T0RrNE55SXNJbTV2YldKeVpWOXRhV055YjNObGNuWnBZMmx2SWpvaVUwVk9VMDlTUlZOZlRsTWlMQ0psYldGcGJGOXlaWE53YjI1ellXSnNaU0k2SW1wMVlXNHVibWxqYUc5NVFIVmhieTVsWkhVdVkyOGlMQ0pwWVhRaU9qRTJNVGM1TXpRNU5ERXNJbVY0Y0NJNk1UWXhOemswTkRrME1IMC5wanBJOXoxUURXZmNwUFBYRHdCYk91enpEcFFhTGk1QlBzd1k5Q0pYR2tJIiwiTUFSQ0EiOiJUZXN0IiwiUkVGRVJFTkNJQSI6IlRlc3QgcmVmIiwiTk9NQlJFX01JQ1JPU0VSVklDSU8iOiJTRU5TT1JFU19OUyIsIkVNQUlMX1JFU1BPTlNBQkxFIjoianVhbi5uaWNob3lAdWFvLmVkdS5jbyIsIkRJU1BPU0lUSVZPX0FDVElWTyI6MSwiaWF0IjoxNjE3OTM3NDA2LCJleHAiOjE2NDk0NzM0MDZ9.8wiuN6xfS_Fc1wPw0UG7HESs7lWHr3QOFvGF5d2cpn8";

//Puerto sensores
#define pulso 9  //define la salida por donde se manda el pulso como 9
#define rebote 8 //define la salida por donde se recibe el rebote como 10

//Variables sensores
int distancia;  //crea la variable "distancia"
float tiempo;  //crea la variable tiempo (como float)
String str;

void setup()
{
  LTask.begin();
  LWiFi.begin();
  Serial.begin(19200);

  //Configuración indicadores
  pinMode(greenPIN , OUTPUT);  //definir pin como salida
  pinMode(redPIN , OUTPUT);  //definir pin como salida
  pinMode(bluePIN , OUTPUT);  //definir pin como salida

  //Configuración puertos
  pinMode(pulso, OUTPUT); //Declaramos el pin 9 como salida (pulso ultrasonido)
  pinMode(rebote, INPUT); //Declaramos el pin 8 como entrada (recepción del pulso)

  // keep retrying until connected to AP
  Serial.println("Trying to connect to WiFi...");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    delay(500);
  }
}

boolean wifi_status(LWifiStatus ws){
  switch(ws){
    case LWIFI_STATUS_DISABLED:
      return false;
    break;
    case LWIFI_STATUS_DISCONNECTED:
      return false;
    break;
    case LWIFI_STATUS_CONNECTED:
      return true;
    break;
  }
  return false;
}


void loop()
{   
  LWiFiClient c;

  while(1){
    LWifiStatus ws = LWiFi.status();
    boolean status = wifi_status(ws);
    if(!status){
      Serial.println("Trying to connect to WiFi...");
      digitalWrite(redPIN , HIGH);
      delay(500);
      while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
      {
        delay(500);
      }
    }else{
      digitalWrite(redPIN , LOW);   
      delay(100); 
      digitalWrite(greenPIN , HIGH);   
      delay(100);   
    }

    digitalWrite(pulso,LOW); //Por cuestión de estabilización del sensor
    delayMicroseconds(5);
    digitalWrite(pulso, HIGH); //envío del pulso ultrasónico
    delayMicroseconds(10);
    tiempo = pulseIn(rebote, HIGH);  //funcion para medir el tiempo y guardarla en la variable "tiempo"
    distancia = 0.01715*tiempo; //fórmula para calcular la distancia
  
    //Enviar datos al ESP32
    StaticJsonDocument<256> doc;
    JsonObject json = doc.to<JsonObject>(); //Se declara un objeto o un JSON vacio

    json["id_nodo_sensor"] = id_nodo_sensor;
    json["token"] = token_dispositivo;

  
    JsonObject variables = json.createNestedObject("variables");
        
    //poner variables para enviar a la plataforma
    variables["nivel_agua"] = distancia;
    String output;
    serializeJsonPretty(json, output); //Se serializa el JSON
    Serial.println(output);

    c.connect(URL,3010);
    c.println("POST /api/publishDatosNodo");  //modify the POST path
    c.print("Host: ");
    c.println(URL);
    c.println("Connection: keep-alive");
    c.println("Content-Type: application/json");
    c.print("Content-Length: ");
    c.println(output.length());
    c.print("Authorization: ");
    c.println(token_autenticacion);
    c.println();
    c.println(output);  //send the HTTP POST body
     
    // waiting for server response
    Serial.println("waiting HTTP response:");
    while (!c.available())
    {
      delay(100);
    }
  }
}
