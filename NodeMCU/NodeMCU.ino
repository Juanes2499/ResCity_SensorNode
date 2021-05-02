

//#include <WiFiEsp.h>
//#include <PubSubClient.h>
//#include <SoftwareSerial.h>

//const int ledPIN = 2;
// 
//void setup() {
//  Serial.begin(9600);    //iniciar puerto serie
//  pinMode(ledPIN , OUTPUT);  //definir pin como salida
//}
// 
//void loop(){
//  digitalWrite(ledPIN , HIGH);   // poner el Pin en HIGH
//  delay(1000);                   // esperar un segundo
//  digitalWrite(ledPIN , LOW);    // poner el Pin en LOW
//  delay(1000);                   // esperar un segundo
//}

#include <WiFi.h>
#include <HTTPClient.h>

//Credenciales de la red WIFI
const char* ssid = "402C";
const char* password = "nichoy99082411425";

//Host del servidor:
String host = "http://192.168.0.17:3010/api";

const int greenPIN = 2;
const int redPIN = 15;

void setup() {
 
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  
  pinMode(greenPIN , OUTPUT);  //definir pin como salida
  pinMode(redPIN , OUTPUT);  //definir pin como salida
 
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

   if (Serial.available()) {
    Serial.write(Serial.read());
  }

  delay(2000);

  if(WiFi.status() == WL_CONNECTED){
    digitalWrite(redPIN , LOW);   // poner el Pin en HIGH
    delay(500); 
    digitalWrite(greenPIN , HIGH);   // poner el Pin en HIGH
    delay(500);                   // esperar un segundo
  }else{
    digitalWrite(greenPIN , LOW);   // poner el Pin en HIGH
    delay(500);                   // esperar un segundo
    digitalWrite(redPIN , HIGH);   // poner el Pin en HIGH
    delay(500); 
  }
      
  if (http.begin(client, host)) //Iniciar conexión
  {
    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();  // Realizar petición
    if (httpCode > 0) {
       Serial.printf("[HTTP] GET... code: %d\n", httpCode);
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();   // Obtener respuesta
          Serial.println(payload);   // Mostrar respuesta por serial
       }
    }
    else {
       Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
  delay(2000);
}
