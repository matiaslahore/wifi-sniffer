import subprocess as sub
import paho.mqtt.client as paho
import json
import sys

broker = "181.15.155.210"
mqttPort = 1883

MAX_RSSI = 60
MAC_SNIFFER = '00:00:00:00:00:00'
CHANNEL = "NODE"

def on_publish(mosq, obj, mid):
    pass

client = paho.Client()
client.on_publish = on_publish
client.connect(broker, mqttPort, 60)
client.subscribe(CHANNEL, 0)

p = sub.Popen(('sudo', 'tcpdump', '-i', 'mon.wlan0', 'type', 'ctl', 'subtype', 'cts'), stdout=sub.PIPE)
for row in iter(p.stdout.readline, b''):
    line = row.rstrip()
    line = line.decode()
    if MAC_SNIFFER in line:
        dbm = int(line.split(" ")[5][1:3])
        if (dbm < MAX_RSSI):
            print("|||||||||||||||||")
            print("MAC FOUND: " + sys.argv[1])
            print("|||||||||||||||||")
            message = {
              "NODE": 'TV',
              "MAC": sys.argv[1],
              "RSSI": dbm
            }
            client.publish(CHANNEL, json.dumps(message))
            break
            os.system('sh killReciverSender.sh')