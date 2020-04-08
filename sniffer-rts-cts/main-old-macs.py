import os
import paho.mqtt.client as paho
import json
import time

broker = "181.15.155.210"
mqttPort = 1883
subscribe = "FIND-OLD-MACS"

def on_message(mosq, obj, msg):
    json_str = msg.payload.decode("utf-8")
    json_response = json.loads(json_str)
    for mac in json_response:
        print("|||||||||||||||||")
        print("FINDING MAC " + mac["mac"])
        print("|||||||||||||||||")
        os.system('python rts-sender-old-macs.py ' + mac['mac'] + ' &')
        os.system('python3 cts-reciver-old-macs.py ' + mac['mac'] + ' &')
        time.sleep(20)
        os.system('for a in $(ps -ef | egrep "cts-reciver-old-macs.py|rts-sender-old-macs.py" | grep -v "grep" | awk \'{print $2}\'); do sudo kill -9 $a; done')
        time.sleep(5)


client = paho.Client()
client.on_message = on_message
client.connect(broker, mqttPort, 60)
client.subscribe(subscribe, 0)
while client.loop() == 0:
    pass

os.system('sudo python3 main-old-macs.py' + ' &')