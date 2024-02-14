#include "SIM7080G.h"
#include <Technofox.h>
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


void SIM7080G::begin(HardwareSerial *serial, uint8_t RX, uint8_t TX) {
  // Initialize serial communication with rxPin and txPin pins
  _serial = serial;
  _serial->begin(115200, SERIAL_8N1, RX, TX);
}

bool SIM7080G::SendPacketData(const char* jsonData) {
  // Envoyez les commandes AT nécessaires via la communication série configurée
  DEBUG_PRINTLN("Envoi des données via AT+SMPUB...");
  _serial->print("AT+SMPUB=\"v1/devices/me/telemetry\",");
  _serial->print(strlen(jsonData));
  _serial->println(",1,1");
  delay(100);
  // Attendez une réponse du module
  String response;
  unsigned long start = millis();
  while (1) {
    if (_serial->available() || (millis() - start) < DELAY_TO_WAIT_RESPONSE) { // Attendre pendant 200 ms (ajustez selon vos besoins)
      String str = _serial->readString();
      response += str;
    } else {
      break;
    }
  }

  // Affichez la réponse du module
  DEBUG_PRINTLN("Réponse du modem : " + response);

  // Si la réponse contient "OK", les données ont été envoyées avec succès
  if (response.indexOf("OK") != -1) {
    DEBUG_PRINTLN("Données envoyées avec succès !");
    return true;
  } else {
    // Échec de l'envoi des données
    DEBUG_PRINTLN("Échec de l'envoi des données.");
    return false;
  }
}


// For integer data
bool SIM7080G::SendData(const char* fieldName, int value) {
  // Create a JSON string with the specified field and integer value
  String jsonMessage = "{\"" + String(fieldName) + "\":" + String(value) + "}";

  // Send the data length to the MQTT server
  int dataLength = jsonMessage.length();
  String dataLengthStr = String(dataLength);

  // Display a debug message with the JSON data to be sent
  DEBUG_PRINTLN("JSON data to send: " + jsonMessage);

  // Display a debug message with the data length
  DEBUG_PRINTLN("Data length: " + dataLengthStr);

  // Send the necessary AT commands via the configured serial communication
  DEBUG_PRINTLN("Sending data via AT+SMPUB...");
  _serial->print("AT+SMPUB=\"v1/devices/me/telemetry\",");
  _serial->print(dataLengthStr);
  _serial->println(",1,1");
  delay(20);

  // Use SendCommand to send the JSON string
  String command = jsonMessage;
  String response = SendCommand(command);

  // Display the module's response
  DEBUG_PRINTLN("Modem response: " + response);

  // If the response contains "OK," the data was sent successfully
  if (response.indexOf("OK") != -1) {
    DEBUG_PRINTLN("Data sent successfully!");
    return true;
  } else {
    // Data sending failed
    DEBUG_PRINTLN("Data sending failed.");
    return false;
  }
}

// For floating-point data
bool SIM7080G::SendData(const char* fieldName, float value) {
  // Create a JSON string with the specified field and float value
  String jsonMessage = "{\"" + String(fieldName) + "\":" + String(value, 2) + "}";

  // Send the data length to the MQTT server
  int dataLength = jsonMessage.length();
  String dataLengthStr = String(dataLength);

  // Display a debug message with the JSON data to be sent
  DEBUG_PRINTLN("JSON data to send: " + jsonMessage);

  // Display a debug message with the data length
  DEBUG_PRINTLN("Data length: " + dataLengthStr);

  // Send the necessary AT commands via the configured serial communication
  DEBUG_PRINTLN("Sending data via AT+SMPUB...");
  _serial->print("AT+SMPUB=\"v1/devices/me/telemetry\",");
  _serial->print(dataLengthStr);
  _serial->println(",1,1");
  delay(20);

  // Use SendCommand to send the JSON string
  String command = jsonMessage;
  String response = SendCommand(command);

  // Display the module's response
  DEBUG_PRINTLN("Modem response: " + response);

  // If the response contains "OK," the data was sent successfully
  if (response.indexOf("OK") != -1) {
    DEBUG_PRINTLN("Data sent successfully!");
    return true;
  } else {
    // Data sending failed
    DEBUG_PRINTLN("Data sending failed.");
    return false;
  }
}
// Pour les données de type String
bool SIM7080G::SendData(const char* fieldName, String value) {
  // Créez une chaîne JSON avec le champ et la valeur spécifiés
  String jsonMessage = "{\"" + String(fieldName) + "\":\"" + value + "\"}";


  // Envoyez la longueur des données au serveur MQTT
  int dataLength = jsonMessage.length();
  String dataLengthStr = String(dataLength);

  // Affichez un message de débogage avec les données JSON à envoyer
  DEBUG_PRINTLN("Données JSON à envoyer : " + jsonMessage);

  // Affichez un message de débogage avec la longueur des données
  DEBUG_PRINTLN("Longueur des données : " + dataLengthStr);

  // Envoyez les commandes AT nécessaires via la communication série configurée
  DEBUG_PRINTLN("Envoi des données via AT+SMPUB...");
  _serial->print("AT+SMPUB=\"v1/devices/me/telemetry\",");
  _serial->print(dataLengthStr);
  _serial->println(",1,1");
    delay(20);
  // Utilisez SendCommand pour envoyer la chaîne JSON
  String command = jsonMessage;
  String response = SendCommand(command);

  // Affichez la réponse du module
  DEBUG_PRINTLN("Réponse du modem : " + response);

  // Si la réponse contient "OK", les données ont été envoyées avec succès
  if (response.indexOf("OK") != -1) {
    DEBUG_PRINTLN("Données envoyées avec succès !");
    return true;
  } else {
    // Échec de l'envoi des données
    DEBUG_PRINTLN("Échec de l'envoi des données.");
    return false;
  }
}


bool SIM7080G::turnOn() {
  // Allumer le module SIM
  // Utilisez _serial pour communiquer avec le module
  digitalWrite(IO_NBIOT_PWRKEY, HIGH); // Mettre la broche IO_NBIOT_PWRKEY à l'état HIGH
  delay(10); // Attendre 100 millisecondes
  digitalWrite(IO_NBIOT_PWRKEY, LOW); // Mettre la broche IO_NBIOT_PWRKEY à l'état LOW
  return true;
}



void SIM7080G::turnOff() {
  DEBUG_PRINTLN("Turn OFF SIMCOM 7080G Module");
  // Switch off the modem using the modem's power button. Keep the button pressed for 3 seconds to switch off normally.
  digitalWrite(IO_NBIOT_PWRKEY, HIGH); // Set IO_NBIOT_PWRKEY pin to HIGH state
  delay(3000); // wait 3000 millisecondes
  digitalWrite(IO_NBIOT_PWRKEY, LOW); // Set IO_NBIOT_PWRKEY pin to LOW state to avoid reboot.

  DEBUG_PRINTLN("Modem is switching off");
}



bool SIM7080G::connect(const char *apn) {
    // 1. Configurez l'APN avec la commande AT+CGDCONT
      String cgdccontCommand = "AT+CGDCONT=1,\"IP\",\"" + String(apn) + "\"";
      DEBUG_PRINTLN("Configuration de l'APN : " + cgdccontCommand);
      String resp = SendCommand(cgdccontCommand);
      DEBUG_PRINTLN(resp);

      // 2. Vérifiez si la carte SIM est prête avec la commande AT+CPIN?
      String cpinCommand = "AT+CPIN?";
      DEBUG_PRINTLN("Vérification de la carte SIM : " + cpinCommand);
      String cpinResponse = SendCommand(cpinCommand);
      DEBUG_PRINTLN(cpinResponse);
      if (cpinResponse.indexOf("OK") == -1) {
        // La carte SIM n'est pas prête, retournez une erreur
        DEBUG_PRINTLN("Problème carte SIM");
        return false;
      }

       // 5. Activez la connexion avec la commande AT+CNACT
      String cnactCommand = "AT+CNACT=0,1";
      DEBUG_PRINTLN("Activation de la connexion GPRS : " + cnactCommand);
      bool cnactSuccess = false;

      // Attendez jusqu'à 30 secondes pour obtenir +APP PDP: 0,ACTIVE
      unsigned long cnactTimeout = millis() + 5000; // Timeout de 30 secondes
      while (millis() < cnactTimeout) {
        String cnactResponse = SendCommand(cnactCommand);
        DEBUG_PRINTLN(cnactResponse);
        if (cnactResponse.indexOf("+APP PDP: 0,ACTIVE") != -1) {
          cnactSuccess = true;
          break; // Connexion établie, sortie de la boucle
        }
        delay(500); // Attendez 0.5 seconde entre les tentatives
      }

      if (!cnactSuccess) {
        // La connexion n'a pas pu être établie, retournez une erreur
        DEBUG_PRINTLN("La connexion n'a pas pu être établie");
        return false;
      }

      // 3. Activez l'attachement GPRS avec la commande AT+CGATT
      String cgattCommand = "AT+CGATT?";
      DEBUG_PRINTLN("Check if device is attached to the network : " + cgattCommand);
      String response = SendCommand(cgattCommand);
      DEBUG_PRINTLN(response);

      // 3. Activez l'attachement GPRS avec la commande AT+CGATT
      cgattCommand = "AT+CNACT?";
      DEBUG_PRINTLN("Check if the App network is active : " + cgattCommand);
      response = SendCommand(cgattCommand);
      DEBUG_PRINTLN(response);

      // 3. Activez l'attachement GPRS avec la commande AT+CGATT
      cgattCommand = "AT+SMSTATE?";
      DEBUG_PRINTLN("Check the connection stuts : " + cgattCommand);
      response = SendCommand(cgattCommand);
      DEBUG_PRINTLN(response);


      // 8. Vérifiez si la connexion au réseau est établie avec la commande AT+CGREG?
      String cgregCommand = "AT+CGREG?";
      DEBUG_PRINTLN("Vérification de la connexion au réseau : " + cgregCommand);
      String cgregResponse = SendCommand(cgregCommand);
      DEBUG_PRINTLN(cgregResponse);

      if (cgregResponse.indexOf("OK") == -1) {
        // La connexion n'est pas établie, retournez une erreur
        DEBUG_PRINTLN("La connexion au réseau n'a pas pu être établie");
        return false;
      }

      return true;
}



bool SIM7080G::checkNetworkRegistration() {
  // Vérifier l'enregistrement au réseau
  // Utilisez _serial pour communiquer avec le module

  // 1. Envoyez la commande AT+CREG? pour vérifier l'enregistrement au réseau
  String cregCommand = "AT+CREG?";
  String cregResponse = SendCommand(cregCommand);

  // Affichez la réponse du module
  DEBUG_PRINTLN("Réponse du module SIM : " + cregResponse);

  // 2. Analysez la réponse pour vérifier l'état d'enregistrement
  if (cregResponse.indexOf("+CREG: 0,1") != -1 || cregResponse.indexOf("+CREG: 0,5") != -1) {
    // Le module est enregistré sur le réseau
    DEBUG_PRINTLN("Le module SIM est enregistré sur le réseau.");
    return true;
  } else if (cregResponse.indexOf("+CREG: 0,2") != -1 || cregResponse.indexOf("+CREG: 0,6") != -1) {
    // Le module est en cours de recherche de réseau
    DEBUG_PRINTLN("Le module SIM est en cours de recherche de réseau.");
    return false;
  } else {
    // L'enregistrement au réseau a échoué
    DEBUG_PRINTLN("Échec de l'enregistrement au réseau.");
    return false;
  }
}


float SIM7080G::getSignal() {
  // Obtenir la force du signal
  // Utilisez _serial pour communiquer avec le module

  // Envoyer la commande AT+CSQ pour obtenir le niveau de signal
  String response = SendCommand("AT+CSQ");

  // Analyser la réponse pour obtenir le niveau de signal
  int debutIndex = response.indexOf("+CSQ: ");
  if (debutIndex != -1) {
    debutIndex += 6; // Avancer le pointeur après "+CSQ: "
    int finIndex = response.indexOf(',', debutIndex); // Trouver la virgule
    if (finIndex != -1) {
      String SignalData = response.substring(debutIndex, finIndex);
      SignalData.trim(); // Supprimer les espaces inutiles

      // Convertir la chaîne en float et la renvoyer
      return SignalData.toFloat();
    }
  }

  // En cas d'échec, renvoyer une valeur invalide (par exemple -1)
  return -1.0;
}



String SIM7080G::getOperator() {
  // Obtenir le nom de l'opérateur
  // Utilisez _serial pour communiquer avec le module

  // Envoyer la commande AT+COPS? pour obtenir le nom de l'opérateur
  String response = SendCommand("AT+COPS?");

  // Vérifier si la réponse contient "+COPS:"
  if (response.indexOf("+COPS:") != -1) {
    // Analyser la réponse pour extraire le nom de l'opérateur
    int debutIndex = response.indexOf('"') + 1;
    int finIndex = response.indexOf('"', debutIndex);
    if (debutIndex != -1 && finIndex != -1) {
      return response.substring(debutIndex, finIndex);
    }
  }

  // En cas d'échec ou de réponse incorrecte, renvoyer une chaîne vide
  return "";
}



String SIM7080G::getIMEI() {
  // Obtenir le numéro IMEI
  // Utilisez _serial pour communiquer avec le module

  // Envoyer la commande AT+GSN pour obtenir le numéro IMEI
  String response = SendCommand("AT+GSN");
  response.replace("\r", ""); // Supprimez les retours chariot
  response.replace("\n", ""); // Supprimez les sauts de ligne
  response.trim(); // Supprimez les espaces inutiles au début et à la fin
  int debutIndex = response.lastIndexOf(' ') +1;

  if(debutIndex != -1) {
   String IMEI = response.substring(debutIndex);

   // Supprimez les caractères indésirables du numéro IMEI
   IMEI.replace("AT+GSN", ""); // Supprimez "AT+GSN" du numéro IMEI
   IMEI.replace("OK", ""); // Supprimez "OK" du numéro IMEI
   IMEI.trim(); // Supprimez les espaces inutiles

      return IMEI;
    }
}


String SIM7080G::getNTP(int UTC) {
  // Formattez la commande AT+CCLK pour définir une date et heure fictive
  String cclkCommand = "AT+CCLK=\"19/04/10,09:27:31+32\"";

  // Envoyez la commande AT+CCLK pour définir une heure fictive sur le module SIM7080G
  if (!SendCommand(cclkCommand)) {
    // En cas d'échec, renvoyez une chaîne vide
    return "";
  }

  // Formattez la commande AT+CNTP pour configurer le serveur NTP avec le décalage horaire
  String cntpCommand = "AT+CNTP=\"time.google.com\"," + String(UTC);

  // Envoyez la commande AT+CNTP pour configurer le serveur NTP
  if (!SendCommand(cntpCommand)) {
    // En cas d'échec, renvoyez une chaîne vide
    return "";
  }

  // Envoyez la commande AT+CNTP pour obtenir l'heure depuis le serveur NTP
  if (!SendCommand("AT+CNTP")) {
    // En cas d'échec, renvoyez une chaîne vide
    return "";
  }

  // Attendez un court délai pour que les données NTP soient reçues
  delay(2000);

  // Envoyez la commande AT+CCLK? pour récupérer l'heure actuelle depuis le module
  String cclkQueryCommand = "AT+CCLK?";
  String response = SendCommand(cclkQueryCommand);

  // Retournez la réponse du module, qui devrait contenir l'heure synchronisée
    DEBUG_PRINTLN(response);
    return response;
}

bool SIM7080G::connectMQTT(const char *clientId, const char *username, const char *password, const char *url, int port) {
  // Configuration du serveur MQTT

  // 1. Configuration du client ID
  String clientIdCommand = "AT+SMCONF=\"CLIENTID\",\"" + String(clientId) + "\"";
  String response = SendCommand(clientIdCommand);
  DEBUG_PRINTLN(response);

  // 2. Configuration de l'URL et du port
  String urlCommand = "AT+SMCONF=\"URL\",\"" + String(url) + "\"," + String(port);
  response = SendCommand(urlCommand);
  DEBUG_PRINTLN(response);

  // 3. Configuration du délai de maintien
  String keepTimeCommand = "AT+SMCONF=\"KEEPTIME\",60";
  response = SendCommand(keepTimeCommand);
  DEBUG_PRINTLN(response);

  // 4. Configuration du nom d'utilisateur
  String usernameCommand = "AT+SMCONF=\"USERNAME\",\"" + String(username) + "\"";
  response = SendCommand(usernameCommand);
  DEBUG_PRINTLN(response);

  // 5. Configuration du mot de passe
  String passwordCommand = "AT+SMCONF=\"PASSWORD\",\"" + String(password) + "\"";
  response = SendCommand(passwordCommand);
  DEBUG_PRINTLN(response);

  // 6. Vérification de la connexion MQTT
  String check = "AT+SMCONN";
  response = SendCommand(check);
  DEBUG_PRINTLN(response);
  if (response.indexOf("OK") != -1) {
    DEBUG_PRINTLN("Connection établie!");
    return true;
    } else {
    DEBUG_PRINTLN("Pas connecté, essaie à nouveau");
    return false;
    }
}


String SIM7080G::SendCommand(String ATCommand) {
  _serial->println(ATCommand); // Send the AT command
  String response;
  unsigned long start = millis();
  while (1) {
    if (_serial->available()) {
      String str = _serial->readString();
      response += str;
      if (str.indexOf("OK") != -1 || str.indexOf("ERROR") != -1) {
        // If the response contains "OK" or "ERROR," exit the loop
        break;
      }
    } else if ((millis() - start) >= DELAY_TO_WAIT_RESPONSE) {
      // If the timeout is reached, exit the loop
      break;
    }
  }

  // Return the complete response for further processing
  return response;
}

  bool SIM7080G::quickAT(String ATCommand) {
    _serial->println(ATCommand); // Send the AT command
    String response;
    unsigned long start = millis();
    while (1) {
      if (_serial->available()) {
        char incomingChar = _serial->read(); // Read a character
        if (isPrintable(incomingChar)) {
          response += incomingChar; // Add the character to the response if printable
        }
        DEBUG_PRINT(incomingChar); // Display the received character
        if (incomingChar == '\n') {
          // Display the response when the line is completed
          DEBUG_PRINT("Response: " + response);
        }
        if (response.indexOf("OK") != -1 || response.indexOf("ERROR") != -1) {
          DEBUG_PRINTLN(); // Print a new line for readability
          return true; // If the response contains "OK" or "ERROR," exit the loop
        }
      } else if ((millis() - start) >= 100) {
        return false; // If the timeout is reached, exit the loop
      }
    }
  }

  bool SIM7080G::checkPower() {
      _serial->println("AT+CPIN?"); // Envoyer la commande AT

      String response;
      bool firstResponse = true; // Ajouter un indicateur pour la première réponse
      unsigned long start = millis();

      while (1) {
          if (_serial->available()) {
              String str = _serial->readString();

              // Vérifier si la réponse contient "OK" ou "ERROR"
              if (str.indexOf("OK") != -1 || str.indexOf("ERROR") != -1) {
                  // Si la réponse contient "OK" ou "ERROR", sortir de la boucle
                  DEBUG_PRINT("Response: ");
                  DEBUG_PRINTLN(str);
                  return true;
              }
          } else if ((millis() - start) >= 500) { // Attendre 500 ms
              // Si le délai est dépassé, tenter de redémarrer et sortir de la boucle
              digitalWrite(IO_NBIOT_PWRKEY, HIGH); // Mettre la broche IO_NBIOT_PWRKEY à l'état HIGH
              delay(10); // Attendre 10 millisecondes
              digitalWrite(IO_NBIOT_PWRKEY, LOW); // Mettre la broche IO_NBIOT_PWRKEY à l'état LOW pour éviter le redémarrage
              DEBUG_PRINT(".");
              return false;
          }
      }
  }


  /* //////////////////////////////////////////////////////////////////////////////////////////////////////
   *
   * This part need to be adapted!!! It's still not working!
   * I need to SUB with a request_ID... not sure how to PUBLISH...
   *
   * This part will be necessary for OTA update also... so very important but not time to do it now
   *
   * //////////////////////////////////////////////////////////////////////////////////////////////*/



  bool SIM7080G::connectTopic(const char *topic) {
    // MQTT Server Configuration

    String sendDataCommand = "AT+SMSUB=\"" + String(topic) + "\",1";
    String response = SendCommand(sendDataCommand);
    DEBUG_PRINTLN(response);
    if (response.indexOf("OK") != -1) {
      DEBUG_PRINTLN("Connection established!");
      return true;
    } else {
      DEBUG_PRINTLN("Not connected, trying again");
      return false;
    }

  }

  bool SIM7080G::unsubscribe(const char *topic) {
      //unsubscribe from topic
      String sendDataCommand = "AT+SMUNSUB=\"" + String(topic) +"\"";
      String response = SendCommand(sendDataCommand);
      DEBUG_PRINTLN(response);
      if (response.indexOf("OK") != -1) {
        DEBUG_PRINTLN("Unsubscribe from the topic!");
        return true;
      } else {
        DEBUG_PRINTLN("Not unsubscribe, please trying again");
        return false;
      }
  }

  bool SIM7080G::disconnect() {
      //unsubscribe from topic
      String sendDataCommand = "AT+SMDISC";
      String response = SendCommand(sendDataCommand);
      DEBUG_PRINTLN(response);
      if (response.indexOf("OK") != -1) {
        DEBUG_PRINTLN("Disconnected!");
        return true;
      } else {
        DEBUG_PRINTLN("Not disconnected, trying again");
        return false;
      }
  }

  // For integer data
  bool SIM7080G::SendDataTopic(const char* fieldName, int value, const char* topic) {
    // Create a JSON string with the specified field and integer value
    String jsonMessage = "{\"" + String(fieldName) + "\":" + String(value) + "}";

    // Send the data length to the MQTT server
    int dataLength = jsonMessage.length();
    String dataLengthStr = String(dataLength);

    // Display a debug message with the JSON data to be sent
    DEBUG_PRINTLN("JSON data to send: " + jsonMessage);

    // Display a debug message with the data length
    DEBUG_PRINTLN("Data length: " + dataLengthStr);

    // Send the necessary AT commands via the configured serial communication
    DEBUG_PRINTLN("Sending data via AT+SMPUB...");
    _serial->print("AT+SMPUB=\"" + String(topic) + "\",");
    _serial->print(dataLengthStr);
    _serial->println(",1,1");
    delay(100);

    // Use SendCommand to send the JSON string
    String command = jsonMessage;
    String response = SendCommand(command);

    // Display the module's response
    DEBUG_PRINTLN("Modem response: " + response);

    // If the response contains "OK," the data was sent successfully
    if (response.indexOf("OK") != -1) {
      DEBUG_PRINTLN("Data sent successfully!");
      return true;
    } else {
      // Data sending failed
      DEBUG_PRINTLN("Data sending failed.");
      return false;
    }
  }


  String SIM7080G::SendDataRequest(const char* topic, String value) {
    // Créer une chaîne JSON avec la valeur spécifiée
    String jsonMessage = String(value);

    // Envoyer les commandes AT nécessaires via la communication série configurée
    _serial->print("AT+SMPUB=\"" + String(topic) + "\",");
    _serial->print(jsonMessage.length());
    _serial->println(",1,1");
    delay(100);

    // Utiliser SendCommand pour envoyer la chaîne JSON
    String command = jsonMessage;
    String response = SendCommand(command);

    // Si la réponse contient "OK" et "+SMSUB: ", les données ont été envoyées avec succès
    if (response.indexOf("OK") != -1 && response.indexOf("+SMSUB: ") != -1) {
      // Trouver la position de la première occurrence de "shared": dans la réponse
      int sharedPos = response.indexOf("shared\":");
      if (sharedPos != -1) {
        // Extraire la sous-chaîne à partir de "shared": jusqu'à la fin
        String jsonData = response.substring(sharedPos + 9); // 9 est la longueur de "shared\":"
        // Trouver la position de la première occurrence de "}}" dans la sous-chaîne
        int endPos = jsonData.indexOf("}}");
        if (endPos != -1) {
          // Extraire la sous-chaîne jusqu'à la position de "}}"
          jsonData = jsonData.substring(0, endPos + 0); // Ajouter 2 pour supprimer les 2 "}}"
        }
        // Afficher les données JSON contenant "shared"
        DEBUG_PRINTLN("JSON data containing 'shared': " + jsonData);
        // Retourner les données JSON contenant "shared"
        return jsonData;
      }
    }

    // Si la réponse indique une erreur ou ne contient pas les données attendues, retourner une chaîne vide
    return "Error";
  }






