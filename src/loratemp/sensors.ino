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

void setup_sensors(){
  
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

float getTemperature(){
    sensors.requestTemperatures();
    return sensors.getTempC(insideThermometer);
}

