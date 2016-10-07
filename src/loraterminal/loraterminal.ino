/*
 * Test application to talk to the RN2483 directly.
 */
#include <SoftwareSerial.h>

SoftwareSerial rnSerial(10, 11); // RX, TX

void setup(void) {
  Serial.begin(9600);
  rnSerial.begin(9600);
  autobaud();

  Serial.println("Init done. Type your command and end with the characters \r\n");
  rnSerial.println("sys get ver");
  Serial.println(rnSerial.readStringUntil('\n'));
  
}

/*
 * Echo bytes back and forth.
 * Don't forget to send carriage return and newline (yes both) after each command.
 */
void loop(void) { 

  // Read from RN2483, write to PC
  if (rnSerial.available()) {
    Serial.write(rnSerial.read());
  }

  // Read from PC, write to RN2483
  if (Serial.available()) {
    byte b = Serial.read();
    rnSerial.write(b);
    Serial.write(b); // echo
  }
}

void autobaud()
{
  //reset rn2483
  Serial.print("Reset radio...");
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(500);
  digitalWrite(12, HIGH);
  Serial.println("done.");
  
  String response = "";
  while (response=="")
  {
    delay(1000);
    Serial.print("autobaud...");
    rnSerial.write((byte)0x00);
    rnSerial.write(0x55);
    rnSerial.println();
    rnSerial.println("sys get ver");
    response = rnSerial.readStringUntil('\n');
    Serial.print(response);
    Serial.println("*");
  }
}
