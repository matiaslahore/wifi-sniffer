import paho.mqtt.client as paho
import json
import sys
#from seguidor import Seguidor
import json
import time
from subprocess import call
import os
import subprocess
import requests

broker = "180.120.10.50"
mqttPort = 1883
mqttUser = "xpmhnkae"
mqttPassword = "qwpGvjcKjtGq"

mac_mati = "48:2c:a0:6a:36:c2"
mac_mati2 = "68:c4:4d:2d:bd:73"
mac_franco = "8C:45:00:B5:60:19"

macToFound = ""
personToFound = ""
send_message = True

def getMacToFound(person_name):
	if (person_name == 'Mati'):
		return mac_mati
	elif (person_name == 'Franco'):
		return mac_franco
	elif (person_name == 'Mati2'):
		return mac_mati2


def on_message(mosq, obj, msg):
	global send_message
	json_str = msg.payload.decode("utf-8")
	json_response = json.loads(json_str)
	macs = json_response["MAC"]
	rssi = json_response["RSSI"]
	device = json_response["NODE"][0]
	channels = json_response["CHANNEL"]
	"""
	print()
	print(device)
	print(macs)
	print()
	"""
	if (macToFound in macs):
		index = macs.index(macToFound)
		print()
		r = requests.post('http://localhost:3000/report', data = {'mac':macToFound, 'device':device})
		print(r.status_code)
		#if (send_message):
			#send_message = False
			#print("enviando mensaje...")
			#subprocess.call("./contacto_garbarino.sh", shell=True)
		if (device == 'Sniffer1' or device == 'Sniffer2' or device == 'Sniffer3' or device == 'Sniffer4'):
			print( personToFound + ' se encuentra en el area: TV' + ' => ' + str(abs(rssi[index])) + ' | Device => ' + device)
		elif (device == 'Sniffer5' or device == 'Sniffer6' or device == 'Sniffer7' or device == 'Sniffer8'):
			print( personToFound + ' se encuentra en el area: HELADERAS' + ' => ' + str(abs(rssi[index])) + ' | Device => ' + device)
		elif (device == 'Sniffer9' or device == 'Sniffer10' or device == 'Sniffer11' or device == 'Sniffer12'):
			print( personToFound + ' se encuentra en el area: A/C' + ' => ' + str(abs(rssi[index])) + ' | Device => ' + device)
		print()
	else:
		print()
		print(device)
		print()

def on_publish(mosq, obj, mid):
    pass
if __name__ == '__main__':
	print("START:")
	print(time.asctime( time.localtime(time.time()) ))
	print()
	personToFound = sys.argv[1]
	macToFound = getMacToFound(personToFound)
	subscribers = sys.argv[2:]
	print(subscribers)
	client = paho.Client()
	#client.username_pw_set(mqttUser, password=mqttPassword)
	client.on_message = on_message
	#client.on_publish = on_publish
	client.connect(broker, mqttPort, 60)
	for suscriber in subscribers:
		client.subscribe(suscriber, 0)
	while client.loop() == 0:
		pass