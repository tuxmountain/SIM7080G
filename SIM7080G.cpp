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

/*
Ok maintenant ça fonctionne... mais il faudra quand même vérifié certain point demain...
1. modifier SendData int et float
2. TurnOn doit plutôt faire un boucle rapide d'envoie de commande AT jusqu'à ce qu'il obtienne une réponse
3. ATZ au lancement de la connection serait pas mal
4. Si CPIN? répond négativement, il faut essayer de reseter le modem et le relancer
*/

void SIM7080G::begin(HardwareSerial *serial, uint8_t RX, uint8_t TX) {
  // Initialise la communication série avec les broches rxPin et txPin
  _serial = serial;
  _serial->begin(115200, SERIAL_8N1, RX, TX);
}

// Pour les données de type float
bool SIM7080G::SendData(const char* fieldName, float value) {
  // Créez une chaîne JSON avec le champ et la valeur spécifiés
  char jsonMessage[100]; // Augmentez la taille si nécessaire
  snprintf(jsonMessage, sizeof(jsonMessage), "{\"%s\":%.2lf}", fieldName, (double)value); // Convertir en JSON la valeur

  // Envoyez la longueur des données au serveur MQTT
  int dataLength = strlen(jsonMessage);
  String dataLengthStr = String(dataLength);

  // Affichez un message de débogage avec les données JSON à envoyer
  DEBUG_PRINTLN("Données JSON à envoyer : " + String(jsonMessage));

  // Affichez un message de débogage avec la longueur des données
  DEBUG_PRINTLN("Longueur des données : " + dataLengthStr);

  // Envoyez les commandes AT nécessaires via la communication série configurée
  DEBUG_PRINTLN("Envoi des données via AT+SMPUB...");
  _serial->print("AT+SMPUB=\"v1/devices/me/telemetry\",");
  _serial->print(dataLengthStr);
  _serial->println(",1,1");

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
// Pour les données de type int
bool SIM7080G::SendData(const char* fieldName, int value) {
  // Créez une chaîne JSON avec le champ et la valeur spécifiés
  char jsonMessage[100]; // Augmentez la taille si nécessaire
  snprintf(jsonMessage, sizeof(jsonMessage), "{\"%s\":%d}", fieldName, value); // Convertir en JSON la valeur

  // Envoyez la longueur des données au serveur MQTT
  int dataLength = strlen(jsonMessage);
  String dataLengthStr = String(dataLength);

  // Affichez un message de débogage avec les données JSON à envoyer
  DEBUG_PRINTLN("Données JSON à envoyer : " + String(jsonMessage));

  // Affichez un message de débogage avec la longueur des données
  DEBUG_PRINTLN("Longueur des données : " + dataLengthStr);

  // Envoyez les commandes AT nécessaires via la communication série configurée
  DEBUG_PRINTLN("Envoi des données via AT+SMPUB...");
  _serial->print("AT+SMPUB=\"v1/devices/me/telemetry\",");
  _serial->print(dataLengthStr);
  _serial->println(",1,1");

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
  delay(200);
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

  // Affichez un message de débogage pour indiquer que le module SIM est en cours d'allumage
  DEBUG_PRINTLN("Allumage du module SIM...");
  String Test = "AT";
  String response= (SendCommand(Test)); // Envoyer la commande AT
  // Attendez une réponse du module
  

  // Affichez la réponse du module
  DEBUG_PRINTLN("Réponse du modem : " + response);

  // Si la réponse du module contient quelque chose, la communication est établie avec succès
  if (!response.isEmpty()) {
    DEBUG_PRINTLN("Module SIM allumé avec succès !");
    return true;
  } else {
    // Échec de l'allumage du module
    DEBUG_PRINTLN("Échec de l'allumage du module SIM.");
    return false;
  }
}





bool SIM7080G::connect(const char *apn, int UTC) {
  // Établir la connexion au réseau avec l'APN spécifié
  // Utilisez _serial pour communiquer avec le module

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
  
  // 4. Obtenez le nom de l'opérateur avec la fonction getOperator
  DEBUG_PRINTLN("Obtention du nom de l'opérateur...");
  String operatorName = getOperator();
  DEBUG_PRINTLN(operatorName);

  // 6. Obtenez la force du signal avec getSignal
  DEBUG_PRINTLN("Obtention de la force du signal...");
  float Signal = getSignal();	
  DEBUG_PRINTLN(Signal);
  	
  // 7. Obtenez l'IMEI avec getIMEI
  DEBUG_PRINTLN("Obtention du numéro IMEI...");
  String IMEI = getIMEI();	
  DEBUG_PRINTLN(IMEI);

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

  // 9. Obtenez l'heure actuelle en interrogeant le module
  DEBUG_PRINTLN("Obtention de l'heure depuis le serveur NTP...");
  String currentDateTime = getNTP(UTC);
  DEBUG_PRINTLN(currentDateTime);	
  // La connexion a été établie avec succès et l'heure a été synchronisée
  DEBUG_PRINTLN("Connexion réussie !");
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




bool SIM7080G::connectTelemetry(const char *clientId, const char *username, const char *password, const char *url, int port) {
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

  // 7. Envoi de données au serveur MQTT
  String sendDataCommand = "AT+SMSUB=\"v1/devices/me/telemetry\",1";
  response = SendCommand(sendDataCommand);
  DEBUG_PRINTLN(response);
}


String SIM7080G::SendCommand(String ATCommand) {
  _serial->println(ATCommand); // Envoyer la commande AT
  String response;
  unsigned long start = millis();
  while (1) {
    if (_serial->available()) {
      String str = _serial->readString();
      response += str;
      if (str.indexOf("OK") != -1 || str.indexOf("ERROR") != -1) {
        // Si la réponse contient "OK" ou "ERROR", sortez de la boucle
        break;
      }
    } else if ((millis() - start) >= DELAY_TO_WAIT_RESPONSE) {
      // Si le délai d'attente est dépassé, sortez de la boucle
      break;
    }
  }

  // Retourner la réponse complète pour traitement ultérieur
  return response;
}




