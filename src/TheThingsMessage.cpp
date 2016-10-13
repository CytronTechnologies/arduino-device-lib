#include <TheThingsMessage.h>

appdata_t TheThingsMessage::initAppData() {
  appdata_t appData = api_AppData_init_default;
  return appData;
}

sensordata_t TheThingsMessage::initSensorData() {
  sensordata_t sensorData = api_SensorData_init_default;
  return sensorData;
}

bool TheThingsMessage::decodeAppData(appdata_t *receiveData, const byte *payload, size_t length) {
  pb_istream_t stream = pb_istream_from_buffer(payload, length);
  if (!pb_decode(&stream, api_AppData_fields, receiveData)) {
    return false;
  }
  if (receiveData->light) {
    digitalWrite(TTN_PIN_LED, HIGH);
  } else {
    digitalWrite(TTN_PIN_LED, LOW);
  }
}

int TheThingsMessage::encodeSensorData(sensordata_t *data, byte **buffer, size_t *size) {
  byte message[51];
  pb_ostream_t sendStream = pb_ostream_from_buffer(message, sizeof(message));
  pb_encode(&sendStream, api_SensorData_fields, data);
  *buffer = message;
  *size = sendStream.bytes_written;
}
