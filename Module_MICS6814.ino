#include <Arduino.h>
#include <Wire.h>
#include "Serial/package.h"

constexpr int port_CO  = GPIO_NUM_27;
constexpr int port_NH3 = GPIO_NUM_26;
constexpr int port_NO2 = GPIO_NUM_25;

const auto this_device = CustomSerial::device_id::MICS_6814_SENSOR;
  
void send_to_wire_on_request();

void setup()
{
  CustomSerial::begin_slave(this_device, send_to_wire_on_request);
}

void loop() { vTaskDelete(NULL); }

void send_to_wire_on_request()
{
  const float co  = map(analogRead(port_CO),  0, 1023.0, 1, 1000);
  const float nh3 = map(analogRead(port_NH3), 0, 1023.0, 1, 500);
  const float no2 = map(analogRead(port_NO2), 0, 1023.0, 5, 1000) * 0.01f;

  CustomSerial::command_package cmd(this_device, 
    "/co",  co,
    "/nh3", nh3,
    "/no2", no2
  );

  CustomSerial::write(cmd);
}
