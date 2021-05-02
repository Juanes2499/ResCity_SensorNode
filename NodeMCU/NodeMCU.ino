#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


//Credenciales de la red WIFI
const char* ssid = "402C";
const char* password = "nichoy99082411425";

//Host del servidor:
String host = "http://192.168.0.17:3010/api/publishDatosNodo";

//Datos del nodo sensor
String id_nodo_sensor = "901ee534-4ca1-4932-a4d8-ef5c4e64dba1";
const char* token_dispositivo = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZF9kaXNwb3NpdGl2byI6IjkwMWVlNTM0LTRjYTEtNDkzMi1hNGQ4LWVmNWM0ZTY0ZGJhMSIsIm1hcmNhIjoiVGVzdCIsInJlZmVyZW5jaWEiOiJUZXN0IHJlZiIsImxhdGl0dWQiOiIxMTExLjk5OSIsImxvbmdpdHVkIjoiLTEuODk4NyIsIm5vbWJyZV9taWNyb3NlcnZpY2lvIjoiU0VOU09SRVNfTlMiLCJlbWFpbF9yZXNwb25zYWJsZSI6Imp1YW4ubmljaG95QHVhby5lZHUuY28iLCJpYXQiOjE2MTg5NTUwNTl9.NprlCdYG18Of5pUVWOKpBnXtPu5s3kSZZ5Bb7g2nVuo";
const char* token_autenticacion = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJJRF9ESVNQT1NJVElWTyI6IjkwMWVlNTM0LTRjYTEtNDkzMi1hNGQ4LWVmNWM0ZTY0ZGJhMSIsIlRPS0VOIjoiZXlKaGJHY2lPaUpJVXpJMU5pSXNJblI1Y0NJNklrcFhWQ0o5LmV5SnBaRjlrYVhOd2IzTnBkR2wyYnlJNklqa3dNV1ZsTlRNMExUUmpZVEV0TkRrek1pMWhOR1E0TFdWbU5XTTBaVFkwWkdKaE1TSXNJbTFoY21OaElqb2lWR1Z6ZENJc0luSmxabVZ5Wlc1amFXRWlPaUpVWlhOMElISmxaaUlzSW14aGRHbDBkV1FpT2lJeE1URXhMams1T1NJc0lteHZibWRwZEhWa0lqb2lMVEV1T0RrNE55SXNJbTV2YldKeVpWOXRhV055YjNObGNuWnBZMmx2SWpvaVUwVk9VMDlTUlZOZlRsTWlMQ0psYldGcGJGOXlaWE53YjI1ellXSnNaU0k2SW1wMVlXNHVibWxqYUc5NVFIVmhieTVsWkhVdVkyOGlMQ0pwWVhRaU9qRTJNVGM1TXpRNU5ERXNJbVY0Y0NJNk1UWXhOemswTkRrME1IMC5wanBJOXoxUURXZmNwUFBYRHdCYk91enpEcFFhTGk1QlBzd1k5Q0pYR2tJIiwiTUFSQ0EiOiJUZXN0IiwiUkVGRVJFTkNJQSI6IlRlc3QgcmVmIiwiTk9NQlJFX01JQ1JPU0VSVklDSU8iOiJTRU5TT1JFU19OUyIsIkVNQUlMX1JFU1BPTlNBQkxFIjoianVhbi5uaWNob3lAdWFvLmVkdS5jbyIsIkRJU1BPU0lUSVZPX0FDVElWTyI6MSwiaWF0IjoxNjE3OTM3NDA2LCJleHAiOjE2NDk0NzM0MDZ9.8wiuN6xfS_Fc1wPw0UG7HESs7lWHr3QOFvGF5d2cpn8";

const int greenPIN = 2;
const int redPIN = 15;
const int bluePIN = 0;

bool messageComplete = false;
String messageEsclavo = "";

void setup() {
 
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  
  pinMode(greenPIN , OUTPUT);  //definir pin como salida
  pinMode(redPIN , OUTPUT);  //definir pin como salida
  pinMode(bluePIN , OUTPUT);  //definir pin como salida

  messageEsclavo.reserve(200);
 
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

  StaticJsonDocument<256> doc;
  JsonObject json = doc.to<JsonObject>();

  if(WiFi.status() == WL_CONNECTED){

    //Control de indicacores conexión WIFI
    digitalWrite(redPIN , LOW);   
    delay(100); 
    digitalWrite(greenPIN , HIGH);   
    delay(100);     

    http.begin(host);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", token_autenticacion);

    json["id_nodo_sensor"] = id_nodo_sensor;
    json["token"] = token_dispositivo;
    
    JsonObject variables = json.createNestedObject("variables");

    if(messageComplete == true){
        DeserializationError error = deserializeJson(doc,messageEsclavo);
        if(error) {
          Serial.print(F("Fallo en deserialziación JSON: "));
          Serial.println(error.c_str());
          return;
        }

        variables["nivel_agua"] = doc["nivel_agua"];

        String output;
        serializeJsonPretty(json, output);
        
        int httpResponseCode = http.POST(output);
    
        if(httpResponseCode ==  HTTP_CODE_OK){
          digitalWrite(bluePIN , HIGH);
          delay(100);    
          http.end();
        }else{
          String payload = http.getString(); 
          Serial.println(payload);
        }
    }
    
    delay(1000);
    
  }else{
    digitalWrite(greenPIN , LOW);   // poner el Pin en HIGH
    delay(100);                   // esperar un segundo
    digitalWrite(redPIN , HIGH);   // poner el Pin en HIGH
    delay(100); 
  }

  digitalWrite(bluePIN , LOW);   
  messageComplete = false;
  messageEsclavo = "";
  delay(3000);
}

void serialEvent(){
  Serial.println("Hola1");
  while(Serial.available() > 0){
    Serial.println("Hola2");
    char inChar = (char)Serial.read();
    messageEsclavo += inChar;
    if(inChar == '}'){
      messageComplete = true;
    }
  }
}
