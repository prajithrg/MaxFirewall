#!/bin/env python

import random
from threading import Thread

try:
	from scapy.all import *
except ImportError, e:
	print "Couldn't find the scapy module."
	sys.exit(1)

def generate_load(length):
    load = ''
    for i in range(length):
        load += chr(random.randint(0,255))
    return load

Tap1Mac = "b8:cd:a7:ff:ff:01"
DFEBotMac = "b8:cd:a7:ff:ff:88"
Tap0IP = "172.16.50.10"
Tap1IP = "172.16.60.10"
SrcPort = 6653
DstPort = 7653

drop_packet = Ether(dst=Tap0Mac,src=DFETopMac,type=0x800)/IP(src=Tap0IP,dst=Tap1IP)/UDP(sport=SrcPort,dport=DstPort)/generate_load(650)
dropThread = Thread(target=sendp, args=(drop_packet,'iface="tap0"',loop=1))

#sendp(send_packet, iface="tap0", loop=1)

accept_packet = Ether(dst=Tap0Mac,src=DFETopMac,type=0x800)/IP(src=Tap0IP,dst=Tap1IP)/TCP(sport=SrcPort,dport=DstPort)/"GET /index.html HTTP/1.0 \n\n"/generate_load(646)
acceptThread = Thread(target=sendp, args=(accept_packet,iface="tap0",loop=1))

dropThread.start()
acceptThread.start()


#sendp(send_packet, iface="tap0")
