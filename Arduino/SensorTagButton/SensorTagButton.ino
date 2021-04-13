/*
  SensorTag Button

  This example scans for BLE peripherals until a TI SensorTag is discovered.
  It then connects to it, discovers the attributes of the 0xffe0 service,
  subscribes to the Simple Key Characteristic (UUID 0xffe1). When a button is
  pressed on the SensorTag a notification is received and the button state is
  outputted to the Serial Monitor when one is pressed.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - TI SensorTag

  This example code is in the public domain.
*/


#include <ArduinoBLE.h>

uint16_t oldCrancRevs;
uint16_t oldCrancEventTime;
uint16_t oldWheelRevs;
uint16_t oldWheelEventTime;
float crancSpeed;
float wheelSpeed;
float circum;
float path;

void setup() {

  circum = 2.5;

  oldCrancRevs = 0;
  oldCrancEventTime = 0;
  oldWheelRevs = 0;
  oldWheelEventTime = 0;
  wheelSpeed = 0.0;
  crancSpeed = 0.0;
  path = 0.0;

  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  Serial.println("BLE Central - SensorTag button");
  Serial.println("Make sure to turn on the device.");

  // start scanning for peripheral
  BLE.scan();
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // Check if the peripheral is a SensorTag, the local name will be:
    // "CC2650 SensorTag"
    if (peripheral.advertisedServiceUuid() == "1816") {
      // stop scanning
      BLE.stopScan();

      monitorSensorTagButtons(peripheral);

      // peripheral disconnected, start scanning again
      BLE.scan();
    }
  }
}

void monitorSensorTagButtons(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");
  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering service 0x1816 ...");
  if (peripheral.discoverService("1816")) {
    Serial.println("Service discovered");
  } else {
    Serial.println("Attribute discovery failed.");
    peripheral.disconnect();

    while (1);
    return;
  }

  // retrieve csc measurement
  BLECharacteristic simpleKeyCharacteristic = peripheral.characteristic("2a5b");

  // subscribe to the simple key characteristic
  Serial.println("Subscribing to simple key characteristic ...");
  if (!simpleKeyCharacteristic) {
    Serial.println("no simple key characteristic found!");
    peripheral.disconnect();
    return;
  } else if (!simpleKeyCharacteristic.canSubscribe()) {
    Serial.println("simple key characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!simpleKeyCharacteristic.subscribe()) {
    Serial.println("subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed");
    Serial.println("Press the right and left buttons on your SensorTag.");
  }

  while (peripheral.connected()) {
    // while the peripheral is connected

    // check if the value of the simple key characteristic has been updated
    if (simpleKeyCharacteristic.valueUpdated()) {
      // yes, get the value, characteristic is 1 byte so use byte value

      //format:
      // Byte: Flags
      // uint32_t: Wheel Revs
      // uint16_t: Last Wheel Event
      // uint16_t: Cranc Revs
      // uint16_t: Last Cranc Event

      int ctDiff;
      int crDiff;
      int wtDiff;
      int wrDiff;

      byte flags;
      uint32_t wheelRevs;
      uint16_t wheelEventTime;
      uint16_t crancRevs;
      uint16_t crancEventTime;

      uint8_t value[20];
      uint8_t vLength;

      vLength = simpleKeyCharacteristic.valueLength();
      simpleKeyCharacteristic.readValue(value, vLength);

      flags = value[0];
      wheelRevs = value[1] + (value[2] << 8) + (value[3] << 16) + (value[4] << 24);
      wheelEventTime = value[5] + (value[6] << 8);
      crancRevs = value[7] + (value[8] << 8);
      crancEventTime = value[9] + (value[10] << 8);



      //simpleKeyCharacteristic.readValue(flags);
      //simpleKeyCharacteristic.readValue(wheelRevs);
      //simpleKeyCharacteristic.readValue(wheelEventTime);
      //simpleKeyCharacteristic.readValue(crancRevs);
      //simpleKeyCharacteristic.readValue(crancEventTime);

      for (int i = 0; i < vLength; i++) {
        //Serial.print(value[i]);Serial.print(" ");
      }
      //Serial.print(" : ");
      //Serial.print(vLength);Serial.print(" ");
      //Serial.print(flags);
      //Serial.print(" Wheel: ");
      //Serial.print(wheelRevs);Serial.print(" ");
      //Serial.print(wheelEventTime);Serial.print(" Cranc: ");
      Serial.print(crancRevs); Serial.print(" ");
      Serial.print(crancEventTime); Serial.print(" => ");

      ctDiff = crancEventTime - oldCrancEventTime;
      crDiff = crancRevs - oldCrancRevs;
      wtDiff = wheelEventTime - oldWheelEventTime;
      wrDiff = wheelRevs - oldWheelRevs;
    
      if ((ctDiff > 0) && (ctDiff < 3000)) {
        crancSpeed = (crDiff * 60.0) / (ctDiff * 0.001);
      } else {
        crancSpeed = 0.0;
      }
      
      if ((wtDiff > 0) && (wtDiff < 3000)) {
        wheelSpeed = (wrDiff * circum) / (wtDiff * 0.001);
      } else {
        wheelSpeed = 0.0;
      }

      oldCrancRevs = crancRevs;
      oldCrancEventTime = crancEventTime;
      oldWheelRevs = wheelRevs;
      oldWheelEventTime = wheelEventTime;



      Serial.print(crDiff); Serial.print(" ");
      Serial.print(ctDiff); Serial.print(" => ");
      Serial.print(crancSpeed); Serial.print(" || ");

      Serial.print(wrDiff); Serial.print(" ");
      Serial.print(wtDiff); Serial.print(" => ");
      Serial.print(wheelSpeed); Serial.println();

      
      //uint8_t value[11];
      //simpleKeyCharacteristic.readValue(value, simpleKeyCharacteristic.valueLength());
      //for(int i=0;i<11;i++){
      //  Serial.print(value[i]);
      //}
      //Serial.println();
    }
  }

  Serial.println("Sensor disconnected!");
}
