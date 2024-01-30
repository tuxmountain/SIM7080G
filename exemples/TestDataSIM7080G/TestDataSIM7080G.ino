#include <Arduino.h>
#include <SIM7080G.h>
#include "Technofox.h"
#define DEBUG 

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

// SIMCOM MODULE
SIM7080G sim;
Technofox myTechnofox;                   // Create an object "Technofox". It will include standard process and function specially designed to the Technofox ESP32 board

// Data Sending
const char* apn = "iot.1nce.net"; // Exemple : "internet"
int utcOffset = 1; // Décalage horaire en heures, Exemple : 1
const char* clientId = ""; // Exemple : "asdfgasdgadsgs"
const char* username = ""; // Exemple : "asdgadsgadsgfdsa"
const char* password = ""; // Exemple : "fadsgadfghadgads"
const char* mqttServer = "" ; // Exemple : "your.mqtt.com"
int mqttPort = 1883;
int start = 0;
String simOperator = "";

void setup() {
  // Initialisation de la communication série
  myTechnofox.begin();

  // Initialisez votre module SIM7080G avec la communication série
  sim.begin(&Serial2, IO_RXD2, IO_TXD2);
  delay(2000);
  DEBUG_PRINTLN("TECHNOFOX                                         [OK]");
  delay(200);

  // Allumez le module SIM
  /*if (sim.turnOn()) {
    USBSerial.println("Module SIM7080G allumé avec succès.");
  } else {
    USBSerial.println("Échec de l'allumage du module SIM7080G.");
  }

  // Connectez-vous au réseau
  if (sim.connect(apn, utcOffset)) {
    USBSerial.println("Connexion au réseau réussie.");

    // Connectez-vous au serveur MQTT avec les informations appropriées
    if (sim.connectTelemetry(clientId, username, password, mqttServer, mqttPort)) {
      USBSerial.println("Connexion MQTT réussie.");

      // Envoyez des données MQTT
      if (sim.SendData("BatmV", mqttPort)) {
        USBSerial.println("Données MQTT envoyées avec succès.");
      } else {
        USBSerial.println("Échec de l'envoi des données MQTT.");
      }
    } else {
      USBSerial.println("Échec de la connexion MQTT.");
    }
  } else {
    USBSerial.println("Échec de la connexion au réseau.");
  }*/
  sim.turnOn();
  delay(10000);
  start = millis();
  
  sim.connect(apn, utcOffset);
  sim.connectTelemetry(clientId, username, password, mqttServer, mqttPort);
}

void loop() {
  simOperator = sim.getOperator();
  sim.SendData("Operator", simOperator);
  delay(2000);
/*// Mettez en attente de nouvelles données depuis USBSerial
  while (USBSerial.available() > 0) {
    // Lisez chaque caractère reçu depuis USBSerial et ajoutez-le à une variable String
    char c = USBSerial.read();
    receivedData += c;
  }

  // Ensuite, vérifiez si vous avez reçu une commande complète (par exemple, une ligne terminée par un saut de ligne)
  if (receivedData.endsWith("\r\n")) {
    // Si vous avez reçu une commande complète, envoyez-la via la fonction SendCommand
    USBSerial.println(sim.SendCommand(receivedData));

    // Effacez la variable receivedData pour préparer la réception de la prochaine commande
    receivedData = "";
  }*/
  }
  /*while (mySerial2.available() > 0) {
    DGOUT.write(mySerial2.read());
    yield();
  }*/

