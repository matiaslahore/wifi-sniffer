#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
// #include <credentials.h>
#include <set>
#include <string>
#include "./functions.h"
#include "./mqtt.h"

#define disable 0
#define enable  1
//#define SENDTIME 10000
#define MAXDEVICES 60
#define JBUFFER 15+ (MAXDEVICES * 40)
#define PURGETIME 100000
#define MINRSSI -95

unsigned int channel = 11;
int clients_known_count_old, aps_known_count_old;
unsigned long sendEntry, deleteEntry;
char jsonString[JBUFFER];
boolean sendMQTT = false;
unsigned long SENDTIME = 20000;

String device[MAXDEVICES];
int nbrDevices = 0;
int reports = 0;
int usedChannels[15];

#ifndef CREDENTIALS
#define mySSID ""
#define myPASSWORD ""
#define instance "MACF-001"
#endif

const char* mqttServer = "180.120.10.50";
const int mqttPort = 1883;
//const char* mqttUser = "xpmhnkae";
//const char* mqttPassword = "qwpGvjcKjtGq";

StaticJsonBuffer<JBUFFER>  jsonBuffer;

void setup() {
  Serial.begin(115200);
  sendEntry = 0;
  wifi_set_opmode(STATION_MODE);            // Promiscuous works only with station mode

  /*
    wifi_set_channel(channel);
    Serial.println("################# SEARCH MODE #################### ");
    wifi_promiscuous_enable(disable);
    wifi_set_promiscuous_rx_cb(promisc_cb_search);   // Set up promiscuous callback
    wifi_promiscuous_enable(enable);

    get_local_macs();
  */

  channel = 11;
  wifi_set_channel(channel);
  wifi_promiscuous_enable(disable);
  wifi_set_promiscuous_rx_cb(promisc_cb);   // Set up promiscuous callback
  wifi_promiscuous_enable(enable);

  delay(1000);
}

void loop() {
  channel = 11;
  wifi_set_channel(channel);

  unsigned long last_com = millis() - sendEntry;
  if ((last_com > SENDTIME) && (clients_known_count > clients_known_count_old)) {
    if ( sendDevices() ) {
      purgeDevice();
    }
    reports++;
    if (reports > 15) {
      ESP.restart();
    }
  }
}

bool sendDevices() {
  Serial.println("################# SEND DEVICE #################### ");
  String deviceMac;

  wifi_promiscuous_enable(disable);
  connectToWiFi();
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.println(client.state());
    }
    yield();
  }

  // Purge json string
  jsonBuffer.clear();
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& mac = root.createNestedArray("MAC");
  JsonArray& rssi = root.createNestedArray("RSSI");
  JsonArray& node = root.createNestedArray("NODE");
  JsonArray& chanel = root.createNestedArray("CHANNEL");
  node.add(instance);

  // Add Clients
  for (int u = 0; u < clients_known_count; u++) {
    deviceMac = formatMac1(clients_known[u].station);
    if (clients_known[u].rssi >= MINRSSI) {
      mac.add(deviceMac);
      rssi.add(clients_known[u].rssi);
      chanel.add(aps_known[u].channel);
    }
  }

  Serial.println();
  Serial.printf("number of devices: %02d\n", mac.size());
  root.prettyPrintTo(Serial);
  root.printTo(jsonString);

  int publishStatus = client.publish(instance, jsonString);
  if ( publishStatus == 1) {
    Serial.println("Successfully published");
  } else {
    Serial.print("ERROR MQTT: "); Serial.println(publishStatus);
    Serial.println(); Serial.println("!!!!! Not published. Please add #define MQTT_MAX_PACKET_SIZE 2048 at the beginning of PubSubClient.h file"); Serial.println();
  }
  client.loop();
  client.disconnect ();
  delay(100);
  wifi_promiscuous_enable(enable);
  sendEntry = millis();
  return publishStatus;
}

void purgeDevice() {
  Serial.println("################ PURGE Device ################### ");
  clientinfo clients_known[MAX_CLIENTS_TRACKED] = {};
  beaconinfo aps_known[MAX_APS_TRACKED] = {};
  clients_known_count = 0;
  clients_known_count_old = 0;
}

void connectToWiFi() {
  delay(10);
  Serial.println(); Serial.print("Connecting to "); Serial.println(mySSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(mySSID, myPASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(""); Serial.println("WiFi connected"); Serial.println("IP address: "); Serial.println(WiFi.localIP());
}
