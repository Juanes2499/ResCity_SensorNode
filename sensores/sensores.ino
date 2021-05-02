#include <Key.h>
#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>//LIBRERIA DEL SENSOR DE HUMEDAD Y TEMPERATURA
#define pulso 3  //define la salida por donde se manda el pulso como 9
#define rebote 4 //define la salida por donde se recibe el rebote como 10
#define in_caudal A1 //define la salida por donde se recibe el caudal como 10
#define DHTPIN 2//define la salida por donde se recibe el sensor DHT como 10
#define DHTTYPE DHT11 //se define el tipo de sensor, el cual es 11
#define in_lluvia 5//se define el tipo de sensor
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(23, 25, 27, 29, 31, 33); //33,31,29,27,25,23
int dht_in;//crea la variable "dht"
int caudal; //crea la variable "caudal"
int lluvia; //crea la variable "lluvia"
int distancia;  //crea la variable "distancia"
float tiempo;  //crea la variable tiempo (como float)

void setup()
{
  Serial.begin(9600);  //inicializa el puerto serie
  pinMode(pulso, OUTPUT); //Declaramos el pin 3 como salida (pulso ultrasonido)
  pinMode(rebote, INPUT); //Declaramos el pin 4 como entrada (recepción del pulso)
  pinMode(lluvia, INPUT); //Declaramos el pin 5 como entrada
  dht.begin();
  lcd.begin(16,2);
}
 
void loop()
{

  TECLA++;

  if(TECLA==6){       //SUMADOR CON REINICIO LCD
    TECLA=0;
  }
  
  int t = dht.readTemperature();  //TOMA DE DATOS TEMPERATURA SENSOR DTH
  int h = dht.readHumidity();     //TOMA DE DATOS HUMEDAD SENSOR DTH
  digitalWrite(pulso,LOW); //Por cuestión de estabilización del sensor
  delayMicroseconds(5);
  digitalWrite(pulso, HIGH); //envío del pulso ultrasónico
  delayMicroseconds(10);
  tiempo = pulseIn(rebote, HIGH);  //funcion para medir el tiempo y guardarla en la variable "tiempo"
  distancia = 0.01715*tiempo; //fórmula para calcular la distancia
   
  //Monitorización en centímetros por el monitor serial/
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  //SENSOR DE CAUDAL REALIZADO CON SERVOMOTOR

  caudal= map(analogRead(in_caudal), 0, 1023, 0, 6000);
  Serial.print("caudal: ");
  Serial.print(caudal);
  Serial.println(" L");

  //SENSOR DE HUMEDAD Y TEMPERATURA

      //Mostramos mensaje con valores actuales de humedad y temperatura
    Serial.print("Humedad relativa: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");

    //SENSOR DE DETECCION DE LLUVIA 

    lluvia=digitalRead(in_lluvia); //lectura del sensor de lluvia 
        if(lluvia==LOW){
            Serial.println("SE DETECTA LLUVIA");
        }else{
          Serial.println("NO SE DETECTA LLUVIA");
        }
       lcd1();
       Serial.println("--------------------------------------------------------");
       delay(4000); //TIEMPO DEL PROCESO
}

void lcd1(){   //funcion para el LCD

  int t = dht.readTemperature(); //SE VUELVE A TOMAR LAS VARIABLES
  int h = dht.readHumidity(); 
  //TECLA=teclado.getKey();
  //TECLA=1;

    if(TECLA==0){                                 //INICIO
        lcd.setCursor(0,0);
        lcd.print("BIENVENIDO");
        lcd.setCursor(0,1);
        lcd.print("192.168.0.17:3010/api           ");
        Serial.println(TECLA);
        
    }else if(TECLA == 1){                         //DISTANCIA EN LCD

        lcd.setCursor(0,0);
        lcd.print("la distancia es=           ");
        lcd.setCursor(0,1);
        lcd.print(distancia);
        lcd.print(" cm                       ");
        Serial.println(TECLA);
        
    }else if(TECLA==2){                         //EL CAUDAL EN LCD
        lcd.setCursor(0,0);
        lcd.print("el caudal es=             ");
        lcd.setCursor(0,1);
        lcd.print(caudal);
        lcd.print(" L                         ");
        Serial.println(TECLA);
      }else if(TECLA==3){                         //HUMEDAD EN LCD
        lcd.setCursor(0,0);
        lcd.print("la humedad es=               ");
        lcd.setCursor(0,1);
        lcd.print(h);
        lcd.print(" %                                      ");
        Serial.println(TECLA);
      }else if(TECLA==4){                         //TEMPERATURA EN LCD
        lcd.setCursor(0,0);
        lcd.print("la temperatura amb=                   ");
        lcd.setCursor(0,1);
        lcd.print(t);
        lcd.print(" *C                                  ");
        Serial.println(TECLA);
      }else if(TECLA==5){                         //LLUVIA EN LCD
        lcd.setCursor(0,0);
        lcd.print(" ¿lluvia?                               ");
        lcd.setCursor(0,1);
        if(lluvia==LOW){
          lcd.print("SE DETECTA LLUVIA                          ");
        }else{
          lcd.print("NO SE DETECTA LLUVIA                        ");
        }
        Serial.println(TECLA);
      }

}
