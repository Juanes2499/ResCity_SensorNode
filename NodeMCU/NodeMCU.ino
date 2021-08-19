#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define RX2 16
#define TX2 17

//Credenciales de la red WIFI
const char* ssid = "412";
const char* password = "99082#Jn*";

//Host del servidor:
String host = "http://192.168.20.22:3010/api/crearDato";

//Datos del nodo sensor
String id_nodo_sensor = "0cca2f5d-f39d-4b2f-90e6-b4b1f8c79664";
//const char* token_dispositivo = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZF9kaXNwb3NpdGl2byI6IjkwMWVlNTM0LTRjYTEtNDkzMi1hNGQ4LWVmNWM0ZTY0ZGJhMSIsIm1hcmNhIjoiVGVzdCIsInJlZmVyZW5jaWEiOiJUZXN0IHJlZiIsImxhdGl0dWQiOiIxMTExLjk5OSIsImxvbmdpdHVkIjoiLTEuODk4NyIsIm5vbWJyZV9taWNyb3NlcnZpY2lvIjoiU0VOU09SRVNfTlMiLCJlbWFpbF9yZXNwb25zYWJsZSI6Imp1YW4ubmljaG95QHVhby5lZHUuY28iLCJpYXQiOjE2MTg5NTUwNTl9.NprlCdYG18Of5pUVWOKpBnXtPu5s3kSZZ5Bb7g2nVuo";
const char* token_autenticacion = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZF9kaXNwb3NpdGl2byI6IjBjY2EyZjVkLWYzOWQtNGIyZi05MGU2LWI0YjFmOGM3OTY2NCIsIm1hcmNhIjoiQXJ1ZGlubyIsInJlZmVyZW5jaWEiOiJEVUUiLCJsYXRpdHVkIjoiMSIsImxvbmdpdHVkIjoiMiIsImVtYWlsX3Jlc3BvbnNhYmxlIjoianVhbi5uaWNob3lAdWFvLmVkdS5jbyIsImlhdCI6MTYyNjM1NTg3OX0.RkoRZghfYZaDD7mgvUnLUk020WgG5PKoVkekTi7e1rs";

const int greenPIN = 2;
const int redPIN = 15;
const int bluePIN = 0;


void setup() {
 
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);

  WiFi.begin(ssid, password);
  
  pinMode(greenPIN , OUTPUT);  //definir pin como salida
  pinMode(redPIN , OUTPUT);  //definir pin como salida
  pinMode(bluePIN , OUTPUT);  //definir pin como salida
 
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(redPIN , HIGH);   // poner el Pin en HIGH
    delay(500);                   // esperar un segundo
    Serial.println("Connecting to WiFi..");
    delay(500);
  } 

  digitalWrite(redPIN , LOW);   // poner el Pin en HIGH
  delay(1000); 
  Serial.println("Wifi Connected");
}

void loop() {
  
  HTTPClient http;
  WiFiClient client;

  StaticJsonDocument<256> doc, doc2;
  JsonObject json = doc.to<JsonObject>(); //Se declara un objeto o un JSON vacio

  //Se valida el estado del WIFI
  if(WiFi.status() == WL_CONNECTED){

    //Control de indicacores conexión WIFI
    digitalWrite(redPIN , LOW);   
    delay(100); 
    digitalWrite(greenPIN , HIGH);   
    delay(100);

     //Headers de la petición POST
    http.begin(host);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", token_autenticacion);

    //Se inicia armando el JSON
    json["id_nodo_sensor"] = id_nodo_sensor;
    //json["token"] = token_dispositivo;

    //Se crea un subobjeto o JSON hijo dentro del JSON principal
    JsonObject variables = json.createNestedObject("variables");

    deserializeJson(doc2, Serial2.readString());
    //poner variables para enviar a la plataforma
    variables["frecuancia_flujo"] = doc2["frecuancia_flujo"];
    variables["flujo_agua"] = doc2["flujo_agua"];
    variables["nivel_agua"] = doc2["nivel_agua"];
    variables["intensidad_lluvia"] = doc2["intensidad_lluvia"];
    variables["estado_lluvia"] = doc2["estado_lluvia"];
    variables["humedad_ambiente"] = doc2["huemdad_ambiental"];
    variables["temperatura_ambiente"] = doc2["temperatura_ambiental"];

    
//    Serial.print("Datos from mega");
//    Serial.println(Serial2.readString());
//    delay(100);

    String output;
    serializeJsonPretty(json, output); //Se serializa el JSON
    Serial.println(output);
    int httpResponseCode = http.POST(output);

    //Control de errores en respuesta del server
    if(httpResponseCode ==  HTTP_CODE_OK){
      digitalWrite(bluePIN , HIGH);
      delay(100);    
      http.end();
    }else{
      String payload = http.getString(); 
      Serial.println(payload);
    }
    
    delay(100);
    
  }else{
    digitalWrite(greenPIN , LOW);   // poner el Pin en HIGH
    delay(100);                   // esperar un segundo
    digitalWrite(redPIN , HIGH);   // poner el Pin en HIGH
    delay(100); 
  }

  digitalWrite(bluePIN , LOW);   
  delay(1500);
}
