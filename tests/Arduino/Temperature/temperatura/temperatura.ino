
#include <DHT_U.h>
#include <DHT.h>

#include <Adafruit_Sensor.h>


#define DATA_PIN 2
#define DHT_TYPE 22
#define SOLAR_PIN 3

DHT dht(DATA_PIN, DHT_TYPE);

void setup(){
  Serial.begin(115200);
  
  dht.begin();
  
}


void loop(){
  
  /*float value = analogRead(SOLAR_PIN);
       float voltaje = (value * 4) / 818;
       Serial.println(voltaje);*/

  if(Serial.available()>0){
    
    char option = Serial.read();
    delay(100);
   
   switch(option){
    case 't':{
       //SE ENVIA LA TEMPERATURA RECOGIDA POR EL SENSOR DHT
       double temperatura = dht.readTemperature();
       delay(100);
       String t = String((int)temperatura);
       Serial.println(t);
    }
       break;
    case 'h':{
       //SE ENVIA LA HUMEDAD RECOGIDA POR EL SENSOR DHT
       double humedad = dht.readHumidity();
       delay(100);
       String h = String((int)humedad);
       Serial.println(h);
    }
    case 's':{
       float value = analogRead(SOLAR_PIN);
       float voltaje = (value * 4) / 818;
       char buffer[6];
       String v = dtostrf(voltaje,10,2,buffer);//String((analogRead(SOLAR_PIN)*4)/818, DEC);
       Serial.println(v);
       //String v = String(voltaje);
       //Serial.println(v);
       
    }
    
       break; 
    default: break;
   }
   
    
  }
  
}




