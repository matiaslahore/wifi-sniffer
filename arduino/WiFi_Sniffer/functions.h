// This-->tab == "functions.h"

// Expose Espressif SDK functionality
extern "C" {
#include "user_interface.h"
  typedef void (*freedom_outside_cb_t)(uint8 status);
  int  wifi_register_send_pkt_freedom_cb(freedom_outside_cb_t cb);
  void wifi_unregister_send_pkt_freedom_cb(void);
  int  wifi_send_pkt_freedom(uint8 *buf, int len, bool sys_seq);
}

#include <ESP8266WiFi.h>
#include "./structures.h"

#define MAX_APS_TRACKED 1
#define MAX_CLIENTS_TRACKED 300

beaconinfo aps_known[MAX_APS_TRACKED];                    // Array to save MACs of known APs
int aps_known_count = 0;                                  // Number of known APs
int nothing_new = 0;

String local_client[MAX_CLIENTS_TRACKED] = {
  "b8:76:3f:6d:9c:84",
  "70:18:8b:9e:62:89",
  "80:3f:5d:09:a5:cc",
  "92:ba:a6:59:29:cc",
  "3c:a0:67:1c:b4:c6",
  "70:1c:e7:73:bc:c5",
  "dc:44:b6:4a:69:93",
  "50:01:d9:92:d6:b6",
  "d4:25:8b:b8:44:c6",
  "f0:24:75:a3:14:e2",
  "2c:d0:5a:4f:11:fd",
  "24:18:1d:4e:9e:8b",
  "aa:2d:64:18:f9:6b",
  "60:01:94:49:e6:57",
  "1c:87:2c:45:f6:36",
  "ee:52:29:4f:47:f7",
  "fa:80:8a:1c:65:fb",
  "96:24:b1:a6:ae:38",
  "c4:da:26:03:17:97",
  "2e:d0:5a:4f:11:fd",
  "c4:da:26:05:be:69",
  "3c:a0:67:1d:29:c1",
  "5c:cf:7f:69:07:3d",
  "b8:57:d8:33:5d:52",
  "18:21:95:b7:e7:a5",
  "5c:e0:c5:69:90:4c",
  "ac:22:0b:4e:85:0e",
  "32:e1:9e:65:fc:42",
  "12:2a:57:46:cf:5c",
  "40:8d:5c:cc:4f:39",
  "24:f6:77:96:80:b6",
  "fc:3f:db:50:b2:ae",
  "5c:e0:c5:f6:68:b9",
  "40:9f:38:23:d1:65",
  "e0:d5:5e:3d:57:9b",
  "2c:d0:5a:e0:9b:08",
  "d8:c4:97:26:78:c5",
  "60:45:cb:1d:75:a6",
  "ac:29:3a:ea:49:20",
  "c4:da:26:06:7d:7f",
  "90:2b:34:ae:06:6c",
  "6c:40:08:32:d1:a9",
  "38:59:f9:16:31:a2",
  "98:9e:63:c7:70:ae",
  "ac:e0:10:c8:d7:61",
  "3c:a0:67:1c:f7:49",
  "94:de:80:a7:de:ec",
  "f0:9f:c2:80:85:39",
  "e6:83:f6:3e:af:24",
  "a4:db:30:64:6e:72",
  "24:ec:99:ea:5a:5f",
  "d4:25:8b:b8:42:7d",
  "d4:ca:6d:43:a9:58",
  "c4:da:26:03:0c:c6",
  "1c:87:2c:45:eb:35",
  "2e:66:9e:fd:85:6c",
  "fe:04:49:83:b1:3d",
  "50:9e:a7:54:32:be",
  "42:45:de:bf:11:d3",
  "24:da:9b:45:75:d7",
  "34:36:3b:2e:e6:89",
  "d2:ee:72:99:fe:99",
  "a4:e4:b8:83:0d:98",
  "00:26:82:46:89:0b",
  "50:bc:96:83:ba:c9",
  "2c:d0:5a:4f:0c:07",
  "d4:f4:6f:a2:70:a3",
  "70:11:24:cd:1d:5b",
  "52:e7:e6:e2:aa:f1",
  "2e:a0:67:1c:f7:49",
  "6e:f1:64:4a:d3:dc",
  "b2:35:7e:e2:86:1e",
  "86:52:f7:76:58:41",
  "70:18:8b:c3:b1:fe",
  "2e:24:92:2c:db:08",
  "d0:fc:cc:cd:d6:3c",
  "12:43:20:19:27:d3",
  "f8:32:e4:00:bd:fe",
  "88:79:7e:d3:fe:19",
  "e2:2d:61:21:2a:cc",
  "32:99:3e:e6:51:d8",
  "66:42:c6:93:10:c5",
  "9a:00:4d:8d:d7:5d",
  "02:95:be:1a:68:10",
  "94:0e:6b:7e:cb:08",
  "24:18:1d:02:51:5c",
  "ca:b0:7f:40:70:a2",
  "d2:83:f3:03:5c:ea",
  "66:71:75:7c:90:28",
  "2c:d0:5a:4d:32:6d",
  "02:ee:a6:b0:9c:42",
  "ba:71:ca:f9:aa:4b",
  "da:a1:19:7c:bf:42",
  "da:a1:19:40:65:e7",
  "20:54:fa:fe:2c:7a",
  "da:a1:19:c9:b6:41",
  "2e:d0:5a:4f:0c:07",
  "2c:0e:3d:e6:b4:f0",
  "da:a1:19:68:db:27",
  "98:10:e8:7f:7d:12",
  "02:17:47:90:23:3e",
  "0e:a7:1b:fd:6d:bc",
  "1a:7b:1f:d2:26:47",
  "a6:93:98:41:5c:c6",
  "d0:d2:b0:45:8e:2b",
  "d6:23:ea:7f:b7:c1",
  "50:9e:a7:98:04:0f",
  "c6:93:d6:ef:14:cf",
  "fe:02:2b:06:26:d8",
  "9e:eb:d9:42:a4:84",
  "02:00:fa:20:63:46",
  "02:1f:a6:13:a1:22",
  "da:a1:19:1c:90:ea",
  "44:1e:a1:6e:81:f0",
  "2c:d0:5a:e0:72:05",
  "60:45:cb:1d:62:ca",
  "68:db:ca:80:a4:39",
  "e0:ac:cb:6f:ae:54",
  "60:01:94:5d:61:05",
  "78:d7:5f:5a:63:d9"
} ;
// Array to save MACs of known local CLIENTs

clientinfo clients_known[MAX_CLIENTS_TRACKED];            // Array to save MACs of known CLIENTs
int clients_known_count = 0;                              // Number of known CLIENTs
int local_client_count = 125;                              // Number of known local CLIENTs

bool exceeded_max_clients_known = false;

int register_beacon(beaconinfo beacon) {}

String formatMac1(uint8_t mac[ETH_MAC_LEN]) {
  String hi = "";
  for (int i = 0; i < ETH_MAC_LEN; i++) {
    if (mac[i] < 16) hi = hi + "0" + String(mac[i], HEX);
    else hi = hi + String(mac[i], HEX);
    if (i < 5) hi = hi + ":";
  }
  return hi;
}

bool is_a_local_mac(String mac_str)
{
  for (int u = 0; u < local_client_count; u++) {
    if (local_client[u] == mac_str) return true;
  }
  return false;
}

int register_client_search(clientinfo &ci) {
  int known = 0;   // Clear known flag

  for (int u = 0; u < clients_known_count; u++)
  {
    if (! memcmp(clients_known[u].station, ci.station, ETH_MAC_LEN)) {
      clients_known[u].lastDiscoveredTime = millis();
      clients_known[u].rssi = ci.rssi;
      known = 1;
      break;
    }
  }

  if (! known) {
    ci.lastDiscoveredTime = millis();
    // search for Assigned AP
    for (int u = 0; u < aps_known_count; u++) {
      if (! memcmp(aps_known[u].bssid, ci.bssid, ETH_MAC_LEN)) {
        ci.channel = aps_known[u].channel;
        break;
      }
    }
    if (ci.channel != 0) {
      memcpy(&clients_known[clients_known_count], &ci, sizeof(ci));
      clients_known_count++;
    }

    if ((unsigned int) clients_known_count >=
        sizeof (clients_known) / sizeof (clients_known[0]) ) {
      Serial.printf("exceeded max clients_known\n");
      exceeded_max_clients_known = true;
      clients_known_count = 0;
    }
  }
  return known;
}

int register_client(clientinfo &ci) {
  int known = 0;   // Clear known flag

  String mac_str = formatMac1(ci.station);
  if (!is_a_local_mac(mac_str)) {
    //exceeded_max_clients_known = true;

    for (int u = 0; u < clients_known_count; u++)
    {
      if (! memcmp(clients_known[u].station, ci.station, ETH_MAC_LEN)) {
        clients_known[u].lastDiscoveredTime = millis();
        clients_known[u].rssi = ci.rssi;
        known = 1;
        break;
      }
    }

    //Uncomment the line below to disable collection of probe requests from randomised MAC's
    //if (ci.channel == -2) known = 1; // This will disable collection of probe requests from randomised MAC's

    if (! known) {
      ci.lastDiscoveredTime = millis();
      // search for Assigned AP
      for (int u = 0; u < aps_known_count; u++) {
        if (! memcmp(aps_known[u].bssid, ci.bssid, ETH_MAC_LEN)) {
          ci.channel = aps_known[u].channel;
          break;
        }
      }
      //if (ci.channel != 0) {
      if (ci.station) {
        memcpy(&clients_known[clients_known_count], &ci, sizeof(ci));
        clients_known_count++;

        Serial.println(mac_str);
      }

      if ((unsigned int) clients_known_count >=
          sizeof (clients_known) / sizeof (clients_known[0]) ) {
        Serial.printf("exceeded max clients_known\n");
        exceeded_max_clients_known = true;
        clients_known_count = 0;
      }
    }
  }
  return known;
}

String print_beacon(beaconinfo beacon) {}

String print_client(clientinfo ci)
{
  String hi = "";
  int u = 0;
  int known = 0;   // Clear known flag
  if (ci.err != 0) {
    // nothing
  } else {
    Serial.printf("CLIENT: ");
    Serial.print(formatMac1(ci.station));  //Mac of device
    Serial.printf(" ==> ");
    Serial.print(formatMac1(ci.ap));   // Mac of connected AP
    Serial.printf("  % 3d", ci.channel);  //used channel
    Serial.printf("   % 4d\r\n", ci.rssi);
    //    }
  }
  return hi;
}

void promisc_cb(uint8_t *buf, uint16_t len)
{
  int i = 0;
  uint16_t seq_n_new = 0;
  if (len == 12) {
    struct RxControl *sniffer = (struct RxControl*) buf;
  } else if (len == 128) {
    struct sniffer_buf2 *sniffer = (struct sniffer_buf2*) buf;
    if ((sniffer->buf[0] == 0x80)) {
      struct beaconinfo beacon = parse_beacon(sniffer->buf, 112, sniffer->rx_ctrl.rssi);
      if (register_beacon(beacon) == 0) {
        //print_beacon(beacon);
        nothing_new = 0;
      }
    } else if ((sniffer->buf[0] == 0x40)) {
      struct clientinfo ci = parse_probe(sniffer->buf, 36, sniffer->rx_ctrl.rssi);
      //if (memcmp(ci.bssid, ci.station, ETH_MAC_LEN)) {
      if (register_client(ci) == 0) {
        //print_client(ci);
        nothing_new = 0;
      }
      //}
    }
  } else {
    struct sniffer_buf *sniffer = (struct sniffer_buf*) buf;
    //Is data or QOS?
    if ((sniffer->buf[0] == 0x08) || (sniffer->buf[0] == 0x88)) {
      struct clientinfo ci = parse_data(sniffer->buf, 36, sniffer->rx_ctrl.rssi, sniffer->rx_ctrl.channel);
      if (memcmp(ci.bssid, ci.station, ETH_MAC_LEN)) {
        if (register_client(ci) == 0) {
          //print_client(ci);
          nothing_new = 0;
        }
      }
    }
  }
}

void promisc_cb_search(uint8_t *buf, uint16_t len)
{
  int i = 0;
  uint16_t seq_n_new = 0;
  if (len == 12) {
    struct RxControl *sniffer = (struct RxControl*) buf;
  } else if (len == 128) {
    struct sniffer_buf2 *sniffer = (struct sniffer_buf2*) buf;
    if ((sniffer->buf[0] == 0x80)) {
      struct beaconinfo beacon = parse_beacon(sniffer->buf, 112, sniffer->rx_ctrl.rssi);
      if (register_beacon(beacon) == 0) {
        //print_beacon(beacon);
        nothing_new = 0;
      }
    } else if ((sniffer->buf[0] == 0x40)) {
      struct clientinfo ci = parse_probe(sniffer->buf, 36, sniffer->rx_ctrl.rssi);
      //if (memcmp(ci.bssid, ci.station, ETH_MAC_LEN)) {
      if (register_client_search(ci) == 0) {
        //print_client(ci);
        nothing_new = 0;
      }
      //}
    }
  } else {
    struct sniffer_buf *sniffer = (struct sniffer_buf*) buf;
    //Is data or QOS?
    if ((sniffer->buf[0] == 0x08) || (sniffer->buf[0] == 0x88)) {
      struct clientinfo ci = parse_data(sniffer->buf, 36, sniffer->rx_ctrl.rssi, sniffer->rx_ctrl.channel);
      if (memcmp(ci.bssid, ci.station, ETH_MAC_LEN)) {
        if (register_client_search(ci) == 0) {
          //print_client(ci);
          nothing_new = 0;
        }
      }
    }
  }
}




