import os
import paho.mqtt.client as paho
import json

broker = "181.15.155.210" #this is for external
mqttPort = 1883
subscribers = ["LOOK-MAC"]

def on_message(mosq, obj, msg):
    json_str = msg.payload.decode("utf-8")
    json_response = json.loads(json_str)
    for mac in json_response:
        print("|||||||||||||||||")
        print("FINDING MAC: " + mac)
        print("|||||||||||||||||")
        os.system('python rts-sender.py ' + mac + ' &')
        os.system('python3 cts-reciver.py ' + mac + ' &')
        os.system('sh killReciverSender.sh')


client = paho.Client()
client.on_message = on_message
client.connect(broker, mqttPort, 60)
for suscriber in subscribers:
    client.subscribe(suscriber, 0)
while client.loop() == 0:
    pass

os.system('python3 main.py' + ' &')