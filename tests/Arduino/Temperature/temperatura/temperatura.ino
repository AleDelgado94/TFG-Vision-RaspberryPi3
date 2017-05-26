
#include <DHT_U.h>
#include <DHT.h>

#include <Adafruit_Sensor.h>


#define DATA_PIN 2
#define DHT_TYPE 11

DHT dht(DATA_PIN, DHT_TYPE);

void setup(){
  Serial.begin(9600);
  
  dht.begin();
  
}


void loop(){
  
  int temperatura = (int)dht.readTemperature();
  int humedad = (int)dht.readHumidity();
  if(Serial.available()>0){
    
    char option = Serial.read();
    delay(100);
   
   switch(option){
    case 't':{
       //SE ENVIA LA TEMPERATURA RECOGIDA POR EL SENSOR DHT
       delay(100);
       Serial.print(temperatura);
    }
       break;
    case 'h':{
       //SE ENVIA LA HUMEDAD RECOGIDA POR EL SENSOR DHT
       delay(100);
       Serial.print(humedad);
    }
       break; 
    default: break;
   }
   
  // delay(500);
    
  }
  
}




