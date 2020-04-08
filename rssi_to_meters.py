import sys
import math as Math

def rssi_to_distance(rssi):
 #hard coded power value. Usually ranges between -59 to -65
 txPower = -47
  
 if (rssi == 0):
 	return -1.0

 ratio = rssi*1.0/txPower
 if (ratio < 1.0):
  	return Math.pow(ratio,10)
 else:
   	distance =  (0.89976)*Math.pow(ratio,7.7095) + 0.111
   	return distance


if __name__ == '__main__':
	print(rssi_to_distance(60))