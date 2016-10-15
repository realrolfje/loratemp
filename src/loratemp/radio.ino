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

TheThingsNetwork ttn(loraSerial, Serial, TTN_FP_EU868);

void setup_radio()  {
  reset_radio();
  loraSerial.begin(9600);
  ttn.autobaud();
  ttn.provision(appEui, appKey);
  ttn.showStatus();

#if defined(REGISTERFIRST)
  Serial.println("You need to generate keys at TTN.");
  ttn.showStatus();
  Serial.println("Halting.");
  while(true){}
#endif

  digitalWrite(LED_PIN, HIGH);
  //the device will attempt a join every second till the join is successfull
  while(!ttn.join(appEui, appKey, 20, 5000)){
      Serial.println("---- backing off for a minute ---");
      delay(60000);
  }
  digitalWrite(LED_PIN, LOW);

  delay(6000);
  ttn.showStatus();

  while(true) {
    send_to_TTN("");
  }
  

}

void reset_radio() {
  //reset rn2483
  Serial.print("Reset RN2483...");
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(500);
  digitalWrite(12, HIGH);
  delay(500);
  Serial.println("done.");
}

int dd = 1;

// the loop routine runs over and over again forever:
void send_to_TTN(String data)  {
  byte payload[1];
  payload[0] = dd;
  // Send it off
  ttn.sendBytes(payload, sizeof(payload));
  delay(20000);
  dd++;
}

