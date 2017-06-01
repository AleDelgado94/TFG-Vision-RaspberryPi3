
#include <DHT_U.h>
#include <DHT.h>

#include <Adafruit_Sensor.h>


#define DATA_PIN 2
#define DHT_TYPE 22

DHT dht(DATA_PIN, DHT_TYPE);

void setup(){
  Serial.begin(115200);
  
  dht.begin();
  
}


void loop(){
  
  double temperatura = dht.readTemperature();
  double humedad = dht.readHumidity();
  String t="";
  String h="";
  if(Serial.available()>0){
    
    char option = Serial.read();
    delay(100);
   
   switch(option){
    case 't':{
       //SE ENVIA LA TEMPERATURA RECOGIDA POR EL SENSOR DHT
       delay(100);
       t = String((int)temperatura);
       Serial.println(t);
    }
       break;
    case 'h':{
       //SE ENVIA LA HUMEDAD RECOGIDA POR EL SENSOR DHT
       delay(100);
       h = String((int)humedad);
       Serial.println(h);
    }
       break; 
    default: break;
   }
   
  // delay(500);
    
  }
  
}




