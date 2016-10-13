/*
 * Test project to get the DS18B20 working. Based on one of the
 * examples in the DallasTemperature project.
 */

#define LED_PIN 13
/*
 * Setup function. Here we do the basics
 */
void setup(void) {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

//  setup_sensors();
  setup_radio();
}

/*
 * Main function. It will request the tempC from the sensors and display on Serial.
 */
void loop(void) { 
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

/*
 * Halts the program, blinks the LED on the
 * Arduino to indicate the error number.
 */
void error(int nr) {
  while (true){
    for(int i = 0; i < nr; i++) {
      digitalWrite(LED_PIN, HIGH); delay(150);
      digitalWrite(LED_PIN, LOW);  delay(150);
    }
    delay(1000);
  }
}

