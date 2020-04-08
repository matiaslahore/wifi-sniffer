import sys
from scapy.all import *
from scapy.all import Dot11,Dot11Beacon,Dot11Elt,RadioTap,sendp

ifacx="mon0"

while True:
    time.sleep(1) #1 second -- .1 is 1/1000 seconds
    Doto11 = Dot11(type=1, subtype=11, addr1=sys.argv[1], ID=0x99)
    pkt = RadioTap()/Doto11
    sendp(pkt, iface=ifacx, realtime=True, verbose=False)

