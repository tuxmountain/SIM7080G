# SIM7080G Library with Thingsboard (Under Development)

## Overview

The SIM7080G Arduino library is designed to facilitate communication with the SIM7080G module, a versatile cellular module for Internet of Things (IoT) applications. 
Please note that this library is currently under development and may require some adaptations for use with specific Arduino or ESP boards. However, it provides a solid foundation for working with 
the SIM7080G module.

For more information about the SIM7080G module, refer to the manufacturer's documentation: SIMCOM SIM7080G.

## Key Features

- Cellular Connectivity: Enables cellular communication for IoT projects using the SIM7080G module.
- Easy Integration: Provides a simplified interface for controlling and interacting with the module.
- Flexible Configuration: Allows configuration of various parameters for your specific IoT application.
- Status Reporting: Retrieve information such as signal strength, network status, and more.

## Usage

1. **Initialization**: Create a SIM7080G object by specifying the hardware serial interface connected to the SIM7080G module. For example, `SIM7080G sim(Serial2);` if you're using Serial2.

2. **Begin Communication**: Initialize communication with the module by calling `sim.begin(baudRate, config);`, where `baudRate` is the desired baud rate, and `config` is the configuration for the hardware serial interface.

3. **Connect to the network**: Send AT commands to the module using the `sim.connect(APN)` to connect and check connection

4. **Sending AT Commands**: Send AT commands to the module using the `sim.SendCommand(ATCommand)` function, where `ATCommand` is the AT command to be sent.

5. **Connect to MQTT**: Connect to MQTT server, specially Thingsboard with `sim.connectMQTT(parameters)`, or `sim.connectTopic(topic)` Telemetry and Attributes are already configurerd

6. **Send Data / Request data**: You can both send Data with `sim.SendCommand(ATCommand)` or request new update on the shares attributes of Thingsboard



## Example

```cpp
#include <Arduino.h>
#include <SIM7080G.h>

SIM7080G sim(Serial2); // Initialize SIM7080G with Serial2

void setup() {
  Serial.begin(9600); // Initialize the serial monitor
  sim.begin(9600, SERIAL_8N1); // Initialize communication with SIM7080G
}

void loop() {
  // Send AT command to check network registration status
  sim.SendCommand("AT+CREG?");
  
  
  // Your IoT application code here
}
