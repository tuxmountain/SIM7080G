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

#define DELAY_TO_WAIT_RESPONSE 20000
#include <Arduino.h>
#include <Technofox.h>


class SIM7080G {
public:
  void begin(HardwareSerial *serial = &Serial2, uint8_t RX = 15, uint8_t TX = 16);
  bool turnOn();
  bool connect(const char *apn, int UTC);
  bool connectTelemetry(const char *clientId, const char *username, const char *password, const char *url, int port);
  bool checkNetworkRegistration();
  float getSignal();
  String getOperator();
  String getIMEI();
  String getNTP(int UTC);
  bool SendData(const char* fieldName, float value);
  bool SendData(const char* fieldName, String value);
  bool SendData(const char* fieldName, int value);
  String SendCommand(String ATCommand); 
  
  
private:
  HardwareSerial *_serial; // Référence vers le port série matériel
};

#endif

