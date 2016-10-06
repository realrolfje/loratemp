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
 */
#include "radiokeys.h"
#if !defined(RADIO_APPEUI)
#define RADIO_APPEUI "not defined"
#define RADIO_APPKEY "not defined"
#define REGISTERFIRST 1
#endif

SoftwareSerial mySerial(10, 11); // RX, TX

//create an instance of the rn2483 library, 
//giving the software serial as port to use
rn2483 myLora(mySerial);

// the setup routine runs once when you press reset:
void setup_radio() 
{

  initialize_radio();

#if defined(REGISTERFIRST)
  Serial.println("You need to generate keys at TTN.");
  print_radio_hweui();
  Serial.println("Halting.");
  while(true){}
#else
  join_TTN();
#endif
}

void initialize_radio() {
  //reset rn2483
  pinMode(RADIO_RESET_PIN, OUTPUT);
  digitalWrite(RADIO_RESET_PIN, LOW);
  delay(500);
  digitalWrite(RADIO_RESET_PIN, HIGH);

  //Autobaud the rn2483 module to 9600. The default would otherwise be 57600.
  mySerial.begin(9600); //serial port to radio
  myLora.autobaud();
}

void print_radio_hweui() {
  //print out the HWEUI so that we can register it via ttnctl
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(myLora.hweui());
  Serial.println("RN2483 firmware version:");
  Serial.println(myLora.sysver());
}

void join_TTN() {
  //OTAA: initOTAA(String AppEUI, String AppKey);
  Serial.println("Joining with EUI ");
  Serial.print(RADIO_APPEUI);
  Serial.print(" and app key ");
  Serial.println(RADIO_APPEUI);
  
  digitalWrite(LED_PIN, HIGH);
  bool join_result = myLora.initOTAA(RADIO_APPEUI, RADIO_APPKEY);

  while(!join_result) {
    Serial.println("Join failed. Retry in one minute. Are your keys correct, and do you have TTN coverage?");

    for (int i=0; i<60; i++) {
      digitalWrite(LED_PIN, LOW);
      delay(500); //delay a minute before retry
      digitalWrite(LED_PIN, HIGH);
      delay(500); //delay a minute before retry
    }
    join_result = myLora.init();
  }
  
  Serial.println("Successfully joined TTN."); 
  digitalWrite(LED_PIN, LOW);
}

// the loop routine runs over and over again forever:
void send_to_TTN(String data)  {
  digitalWrite(LED_PIN, HIGH);
  
  Serial.print("Sending to TTN: ");
  Serial.print(data);
  Serial.print("...");
  myLora.tx(data); //one byte, blocking function
  Serial.println("done.");
    
  digitalWrite(LED_PIN, LOW);
}

