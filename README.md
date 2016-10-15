LoRaTemp
========

Arduino LoRaWan project for logging temperatures.

Code for reading temperatures from a [DS18B20 temperature sensor](documentation/DS18B20-Datasheet.pdf) was inspired by the [hobbytronics DS1820 Temperature Sensor](http://www.hobbytronics.co.uk/ds18b20-arduino) blog.

The LoRaWan part of this project will be based on the [MicroChip RN2483](https://www.microchip.com/wwwproducts/en/RN2483) module. The [datasheet](documentation/RN2483-Datasheet.pdf) and [userguide](documentation/RN2483-Userguide.pdf) are stored in the documentation folder of this project, together with other interesting documents and links.

Testing the Dallas Temperature sensor
-------------------------------------

The [DS18B20-test](src/DS18B20-test) project contains a little test project to get the temperature sensor working on pin D2 of the Arduino. For this to work you need to install the [Arduino-Temperature-Control-Library](src/libraries/Arduino-Temperature-Control-Library) and [OneWire](src/libraries/OneWire) library into your Arduino IDE. After uploading the project to your Arduino it should start logging temperatures.

Driving the 3.3V RN2483 from a 5V Arduino:
------------------------------------------

For the input pins on the RN2483, the pin leakage is specified at 50nA max. The pins are clamped with diodes against Vdd, and the clamp diodes can handle 20mA current max. This means we can send our bits to the RN2483 by using a simple resistor, making use of the clamping diodes of the RN2483. To go from 5V to 3.3V at 20mA would require a, 85 Ohm resistor, but that would be silly and risky. So I'd advise a 1k resistor on the RN2483 input pins.

For the output pins it's a bit different. In our case we use an Arduino Nano, which uses a [ATmega 328](http://www.atmel.com/images/Atmel-8271-8-bit-AVR-Microcontroller-ATmega48A-48PA-88A-88PA-168A-168PA-328-328P_datasheet_Complete.pdf). At 5V Vdd, the minimum voltage for a [logical "1"](https://learn.sparkfun.com/tutorials/logic-levels) is 3V. The RN2483 can drive 200mA for all pins combined, so we need to keep the current per pin roughly two orders of magnitude below that, 2mA. The internal pull-up resistors of the Arduino is somewhere between 20k and 50k, and would be safe to enable to pull the logical "1" up through the clamping diode. This will get the output up tp 3.3V + 0.6V = 3.9V. To prevent damage to either chip when misconfiguring, we add a 1k resistor on those lines too (Voltage calculation is left as an exercise to the reader).

Long story short: Connect all data lines between the 5V Arduino and the 3.3V RN2483 with 1k resistors and you won't break stuff.

Connecting the RN2483 to the Arduino
------------------------------------

Connect the RN2483 as follows:

| Arduino pin | RN2483 pin      | Remarks           |
|-------------|-----------------|-------------------|
|        3.3V | 34 and 12 - Vdd | direct            |
|          10 |     6 - UART TX | 1k resistor       |
|          11 |     7 - UART RX | 1k resistor       |
|          12 |      32 - Reset | 1k resistor       |
|         Gnd |             Gnd | direct, see below |

*A note on grounding*: The RN2483 has a lot of Gnd pins, and all of them are electrically connected. For normal power, connecting the Gnd pins on the side of the module to the ground of the Arduino will suffice. However, please take special care that the antenna uses the ground pins next to the antenna outputs. Please see page 10 of the [RN2383 datasheet](documentation/RN2483-Datasheet.pdf) to see how Microchip connects antennas. Do not connect the two antenna outputs to one single antenna.

Notes
-----

Install mosquitto broker: sudo apt-get install mosquitto (not needed because TTN hosts that)
Install mosquitto clients: sudo apt-get install mosquitto-clients

Links to The Things Network (because their site navigation is, let's say "sub optimal")

- [Your TTN user profile](https://account.thethingsnetwork.org/users/profile)
- [All your TTN applications](https://staging.thethingsnetwork.org/applications) Register your
  device and monitor the devices and their sent packets.
  
Help, tips and tricks:
  
- [TTN Forum](https://www.thethingsnetwork.org/forum/)
- [TTN Gateway Map](https://www.thethingsnetwork.org/map)
- [Another Gateway Map](http://ha-23.eradus.eu/croft.html)  
- [Effects of antenna on RSSI](https://www.thethingsnetwork.org/community/tehran/post/effect-of-different-antennas-on-the-rssi#!)
- 

