/*
#include <Wire.h>
#include <Adafruit_AM2315.h>

Adafruit_AM2315 am2315;

void setup() {
   Serial.begin(9600);
   Serial.println("AM2315 Test!");
   
   if (! am2315.begin()) {
      Serial.println("Sensor not found, check wiring & pullups!");
      while(1);
   }
}

void loop() {
   //Serial.println("hello moto");
   Serial.print("Hum: "); Serial.println(am2315.readHumidity());
   Serial.print("Temp: "); Serial.println(am2315.readTemperature());

   delay(1000);
}
*/
