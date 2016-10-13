#include <TheThingsNetwork.h>
#include <TheThingsMessage.h>

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn(loraSerial, debugSerial, /* TTN_FP_EU868 or TTN_FP_US915 */);

sensordata_t sensorData = TheThingsMessage::initSensorData();

void setup()
{
  pinMode(TTN_PIN_LED, INPUT);

  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  ttn.onMessage(message);

  sensorData.has_motion = true;
  sensorData.has_water = true;
  sensorData.has_temperature_celcius = false;
  sensorData.has_temperature_fahrenheit = false;
  sensorData.has_humidity = false;
}

void loop() {
  // Read sensors
  bool motion = digitalRead(TTN_PIN_LED) == HIGH;
  uint32_t water = 682 - analogRead(A0);

  // Init data
  sensorData.motion = motion;
  sensorData.water = water;

  // Encode data
  byte *buffer;
  size_t size;

  // Send standard message on port 100
  TheThingsMessage::encodeSensorData(&sensorData, &buffer, &size);
  ttn.sendBytes(buffer, size, 100);

  delay(10000);
}

void message(const byte* payload, size_t length, port_t port) {
  //standard message always received on port 100 or more
  if (port >= 100) {
    appdata_t appData = TheThingsMessage::initAppData();
    TheThingsMessage::decodeAppData(&appData, payload, length);
  }
}