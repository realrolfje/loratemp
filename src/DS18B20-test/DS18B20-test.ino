/*
 * Test project to get the DS18B20 working. Based on one of the
 * examples in the DallasTemperature project.
 */
#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20 is connected to pin 2 of the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Setup a DallasTemperature object to communicate with OneWire.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

/*
 * Setup function. Here we do the basics
 */
void setup(void)
{
  Serial.begin(9600);
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();

  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  if (!sensors.getAddress(insideThermometer, 0)) {
    Serial.println("Unable to find address for Device 0"); 
    error(1);
  }
  
  sensors.setResolution(insideThermometer, 12);
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();
}

/*
 * Main function. It will request the tempC from the sensors and display on Serial.
 */
void loop(void)
{ 
  float oldTemp = -1;

  while (true) {
    float newTemp = getTemperature();
    if(abs(oldTemp - newTemp) > 0.1) {
      oldTemp = newTemp;
      Serial.print(millis()/1000);
      Serial.print(" - Temp C: ");
      Serial.println(newTemp);
    }
  }
}

float getTemperature(){
    sensors.requestTemperatures();
    return sensors.getTempC(insideThermometer);
}

/*
 * Halts the program, blinks the LED on the
 * Arduino to indicate the error number.
 */
void error(int nr) {
  pinMode(13, OUTPUT);
  while (true){
    for(int i = 0; i < nr; i++) {
      digitalWrite(13, HIGH); delay(150);
      digitalWrite(13, LOW);  delay(150);
    }
    delay(1000);
  }
}

