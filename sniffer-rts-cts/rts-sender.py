import sys
from scapy.all import *
from scapy.all import Dot11,Dot11Beacon,Dot11Elt,RadioTap,sendp

#addr1='68:c4:4d:2d:bd:73' #Mio Chiquito
#addr='48:2c:a0:6a:36:c2' #Mio
#addr='2C:0E:3D:A0:51:CF' #Juan
#addr='8C:45:00:B5:60:19' #Franco

ifacx="mon.wlan0"

while True:
    time.sleep(1) #1 second -- .1 is 1/1000 seconds
    Doto11 = Dot11(type=1, subtype=11, addr1=sys.argv[1], ID=0x99)
    pkt = RadioTap()/Doto11
    sendp(pkt, iface=ifacx, realtime=True, verbose=False)

