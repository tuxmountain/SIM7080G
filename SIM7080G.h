#ifndef SIM7080G_H
#define SIM7080G_H
//For debug purpose, compiler commands

#ifdef  DEBUG
  #define DEBUG_PRINT(x)   USBSerial.print(x)
  #define DEBUG_PRINTLN(x) USBSerial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif
#ifdef  LOG
  #define LOG_PRINT(x)   USBSerial.print(x)
  #define LOG_PRINTLN(x) USBSerial.println(x)
#else
  #define LOG_PRINT(x)
  #define LOG_PRINTLN(x)
#endif
#ifdef  LCD
  #define LCD_PRINT(x)   lcd.print(x)
  #define LCD_PRINTLN(x) lcd.println(x)
#else
  #define LCD_PRINT(x)
  #define LCD_PRINTLN(x)
#endif

#define DELAY_TO_WAIT_RESPONSE 10000 // Defines a maximum waiting time (Time Out) in millisecond for the modem to return a complete response with OK or ERROR. It's important to leave enough time, as the response time may vary depending on the commands. When the response is complete, this parameter is ignored.
#include <Arduino.h>
#include <Technofox.h>


class SIM7080G {
public:
  void begin(HardwareSerial *serial = &Serial2, uint8_t RX = 15, uint8_t TX = 16); //should allow the card to be connected automatically or by passing the parameters
  bool turnOn(); //To restart the board if it is switched off, e.g. after a reboot or loss of power

   /* To check if the modem respond. I recommend using a loop directly in the firmware when you need to communicate with the modem directly.
    * Initialization time can vary up to 60 seconds. It's better to start the modem, perform other operations and come back to check only
    * when you're ready to send data, to avoid blocking the code too long seconds.
    *
    * Exemple:
    * sim.turnOn();
    * while (!sim.checkPower()) {
    *   delay(100); // Wait for 1 second before checking again
    *   }
    * sim.connect(apn, utcOffset);
    * sim.connectTelemetry(clientId, username, password, mqttServer, mqttPort);
 */

  bool checkPower(); // To check if modem respond
  bool SendPacketData(const char* jsonData); // to send package data on telemetry
  void turnOff(); //To simply switch off the card when it is no longer in use
  bool connect(const char *apn); //Automatically connects to the network. All you need to do is specify the APN of the SIM card provider and, if required, the time offset based on UTC. If no selection is made, it will take UTC time.
  bool connectMQTT(const char *clientId, const char *username, const char *password, const char *url, int port); //Connects to the MQTT Thingsboard server. ClientID, Username and Password must be specified, as well as URL and communication port.
  bool checkNetworkRegistration(); //Checks whether you are connected to the network
  float getSignal(); // Allows you to obtain the signal value directly as a float
  String getOperator(); // Gets the name of the network access provider in a String
  String getIMEI(); // Allows you to obtain the unique IMEI number directly in a String
  String getNTP(int UTC); // Allows you to obtain the time with time zone correction by specifying UTC
  bool SendData(const char* fieldName, float value); // Sends data to Thingsboard's Telemetry folder in float
  bool SendData(const char* fieldName, String value); // Sends data to Thingsboard's Telemetry folder in String
  bool SendData(const char* fieldName, int value); //Sends data to Thingsboard's Telemetry folder in int
  String SendCommand(String ATCommand);  //Sends specific AT commands. The \r\n has already passed
  bool quickAT(String ATCommand); //Sending quick AT query to check response from modem
  bool unsubscribe(const char *topic); //Just a command to unsubscribe from the Packet
  bool disconnect(); // Just a command to disconnect from the MQTT Server
  bool connectTopic(const char *topic); //Choose the topic
  bool SendDataTopic(const char* fieldName, int value, const char* topic); //Test for chaning topic
  bool readAttributes(const char* topic);
  String SendDataRequest(const char* topic, String value);
  //Thingsboard Attributes
  bool connectAttributes(const char* url, const char* accessToken, int port = 1883);
  bool publishAttributes(const char* attributes);
  bool subscribeToAttributes();


private:
  HardwareSerial *_serial; // Référence vers le port série matériel
};

#endif

