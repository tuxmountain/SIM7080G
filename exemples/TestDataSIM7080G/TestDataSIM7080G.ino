#include <Arduino.h>
#include <SIM7080G.h>
#include <Preferences.h>
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
Preferences flash; // initialise object for the flash mermory

// Data Sending
const char* apn = ""; // Exemple : "iot.1nce.net"
int utcOffset = 1; // Décalage horaire en heures, Exemple : 1
const char* clientId = ""; // Exemple : "g68lny14n3gitibkmo86"
const char* username = ""; // Exemple : "xk4k3fkxuxszt7u2mhx5"
const char* password = ""; // Exemple : "o4x21qw26h76amw6vhy8"
const char* mqttServer = "" ; // Exemple : "iot.technofox-international.com"
const char* telemetry ="v1/devices/me/telemetry";
const char* attributes ="v1/devices/me/attributes/response/+";
const char* requestTopic ="v1/devices/me/attributes/request/1"; // This is used to send request of new data
int mqttPort = 1883;
int start = 0;
String simOperator = "";
int IntValue = 0;
float FloatValue = 99.99;
int retryCount = 0;
int simActive = 0;

void setup() {
  USBSerial.begin(115200); // You can also use Serial.print depending on your setup
  sim.begin(&Serial2, IO_RXD2, IO_TXD2); // To initialise the connection, please change RX/TX Pin with your own configuration

  sim.turnOff(); // if you want to reset the SIMCOM Module
  delay(1000);
   while (!simActive && retryCount < 20) {
        // Check if we get some respond from the module
        simActive = sim.checkPower();

        if (!simActive) {
            // If sim is not ready, wait a bit for a new attempt
            delay(500); 
            retryCount++;
        }
    }
  sim.turnOn(); // When we check connection, the function will also turnOn the module, but can be helpfull to use this command
  start = millis();
  

  sim.connect(apn); // Connect to the network
  delay(1);
    if(sim.connectMQTT(clientId, username, password, mqttServer, mqttPort) == 1) { // Connect to MQTT server
      DEBUG_PRINTLN("Connected");
    } else {
      sim.connectMQTT(clientId, username, password, mqttServer, mqttPort); // Second try if it failed
      DEBUG_PRINTLN("Try again!");
      }
        delay(1);
        if(sim.connectTopic(telemetry) == 1) {  // Try to connect to telemetry 
          DEBUG_PRINTLN("I'm connected to telemetry");
          simOperator = sim.getOperator(); // get the operator when you're connected
          sim.SendData("Operator", simOperator); // send the operator value on your server to make a first test
        } else {
          DEBUG_PRINTLN("try again to connect to telemetry"); // you can try a second time if necessary to connect to telemetry (subscribe)
          sim.connectTopic(telemetry);
        }
}

void loop() {
  
  sim.SendData("Operator", simOperator); // test to send a string 
  sim.SendData("intvalue", IntValue);  // test to send a int value
  sim.SendData("floatvalue", FloatValue);  // tes to send a float value

  // Please be carefull, key should be shorter than 14 caracters, otherwise it will failed. Value can be at leat 100...

  //Share attributes from Thingsboard
  delay(1000);
  sim.unsubscribe(attributes); // it's necessary to connect first to Telemetry and send at least 1 data! After you can unsubscribe from both topic to be safe
  sim.unsubscribe(telemetry); // disconnect from telemetry
  
  
  if(sim.connectTopic(attributes) == 1) { // Connect to attributes topic
    USBSerial.println("Connected to attributes, ready to receveid update");
    RequestUpdate(); // An exemple of Request
    }
    sim.unsubscribe(attributes); // disconnect
    //sim.disconnect(); // you can also use this function

// If you want to play with AT command, you can also use the follwing code to open a serial monitor to communicate directly with the module

/*
  while (USBSerial.available() > 0) {
    // Read all caracters and put into a String
    char c = USBSerial.read();
    receivedData += c;
  }

  // check if the string is complete and finish with CR to be valide
  if (receivedData.endsWith("\r\n")) {
    // if the data is complet you can send the command
    USBSerial.println(sim.SendCommand(receivedData));

    // clear the variable
    receivedData = "";
  }*/

}
 

void RequestUpdate() {
  
  String RequestedData = "";

  // Liste des requêtes à effectuer
  String requests[] = {
    "{\"sharedKeys\":\"key1,key2,key3,key4,key5,key6\"}",
    "{\"sharedKeys\":\"key1,key2,key3,key4,key5,key6\"}",// you can add as much ine as you want, I advise to don't put more than 6 requests per line.
  };

  for (int i = 0; i < sizeof(requests) / sizeof(requests[0]); i++) {
    String receivedData;
    String request = requests[i];

    // Try again until the response is passed
    while (true) {
      receivedData = sim.SendDataRequest(requestTopic, request);
      if (receivedData != "Error") {
        break; // Go out of the the loop if data was correctly send
      } else {
        DEBUG_PRINTLN("Error received. Retrying request...");
        delay(1000); // wait a bit before sending a new request
      }
    }

    // Add all data on the same String with a "," to seperate key
    RequestedData += receivedData + ",";
  }
  // remove the last comma
  if (RequestedData.endsWith(",")) {
    RequestedData.remove(RequestedData.length() - 1);
  }

  // now you can show all the data or save to flash for exemple
  UpdateFlashData(RequestedData);
}


void UpdateFlashData(const String& jsonData) {
  
  // Create a new String
  String data = jsonData;

  // Clean the data by removing ""
  data.replace("\"", "");

  // Create key & value to prepare to store value on flash
  String key;
  String valueStr;
  
  // While the Json isn't empty, continue to prepare value
  while (data.length() > 0) {
    // search for the first ":" to separate value & key
    int colonPos = data.indexOf(':');
    
    // Extract key value
    key = data.substring(0, colonPos);
    
    // extract the data attached to the key
    data = data.substring(colonPos + 1);
    
    // find the next coma to separate the next value
    int commaPos = data.indexOf(',');
    
    // If there is still a comma, you can extract the next value
    if (commaPos != -1) {
      valueStr = data.substring(0, commaPos);
      
      // remove the chain with the , 
      data = data.substring(commaPos + 1);
    }
    // if there is no "," you can finish to extract value
    else {
      valueStr = data;
      
      // empty the data value
      data = "";
    }

    // Open flash memory in writting mode
    flash.begin("config", false); // WRITING MODE


    // Check if the value contain letters
    bool hasLetters = false;
    for (int i = 0; i < valueStr.length(); ++i) {
      if (isAlpha(valueStr.charAt(i))) {
        hasLetters = true;
        break;
      }
    }

    // if there is some letter, this is a String 
    if (hasLetters) {
      // Update the flash with this String
      flash.putString(key.c_str(), valueStr.c_str());
      
      // show the update
      DEBUG_PRINTLN(key + " = " + valueStr + ";");
    } 
    // check if the value is a float, separate with .
    else if (valueStr.indexOf('.') != -1) {
      // La valeur est un float
      float floatValue = valueStr.toFloat();
      
      // Update the float value
      flash.putFloat(key.c_str(), floatValue);
      
      // show the update
      DEBUG_PRINTLN(key + " = " + String(floatValue) + ";");
    } 
    // if this is not a String or a float, this is a int
    else {
      
      int intValue = valueStr.toInt();
      
      // update the flash memery with int value
      flash.putInt(key.c_str(), intValue);
      
      // show the update
      DEBUG_PRINTLN(key + " = " + String(intValue) + ";");
    }
    flash.end(); // Close the flash memory when it's done
  }
}

