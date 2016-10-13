#include <rn2483.h>
#include <SoftwareSerial.h>

#define RADIO_RESET_PIN 12

/*
 * Please create a radiokeys.h file containing the keys you got
 * when registering with TTN. It should look somewhat like:
 * 
 * #define RADIO_APPEUI "40B3D17ED00301A6"
 * #define RADIO_APPKEY "F23C56EE13304963B8C2BF6283448298"
 * 
 * Register your application in the dashboard of TTN at
 * https://staging.thethingsnetwork.org/applications
 * 
 */
#include "radiokeys.h"
#include <TheThingsNetwork.h>

//#if !defined(appEui)
//#define REGISTERFIRST 1
//const byte appEui[8]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//const byte appKey[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//#endif

SoftwareSerial loraSerial(10, 11); // RX, TX
TheThingsNetwork ttn;

void setup_radio()  {
  loraSerial.begin(9600);
  delay(1000);
  reset_radio();

  ttn.init(loraSerial, Serial);  
  ttn.reset();

#if defined(REGISTERFIRST)
  Serial.println("You need to generate keys at TTN.");
  ttn.showStatus();
  Serial.println("Halting.");
  while(true){}
#endif

  digitalWrite(LED_PIN, HIGH);
  //the device will attempt a join every second till the join is successfull
  while(!ttn.join(appEui, appKey)){
      delay(10000);
  }
  digitalWrite(LED_PIN, LOW);

  delay(6000);
  ttn.showStatus();

}

void reset_radio() {
  //reset rn2483
  Serial.print("Reset RN2483...");
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
    loraSerial.write((byte)0x00);
    loraSerial.write(0x55);
    loraSerial.println();
    loraSerial.println("sys get ver");
    response = loraSerial.readStringUntil('\n');
    Serial.print(response);
  }
  Serial.println("..done.");
}


// the loop routine runs over and over again forever:
void send_to_TTN(String data)  {
  ttn.sendString(data);
  delay(20000);
}

