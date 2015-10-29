#!/bin/env python

import random

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

Tap0Mac = "b8:cd:a7:ff:ff:80"
DFETopMac = "b8:cd:a7:ff:ff:00"
Tap0IP = "172.16.50.10"
Tap1IP = "172.16.60.10"
SrcPort = 6653
DstPort = 7653

send_packet = Ether(dst=Tap0Mac,src=DFETopMac,type=0x800)/IP(src=Tap0IP,dst=Tap1IP)/UDP(sport=SrcPort,dport=DstPort)/"GET /index.html HTTP/1.0 \n\n"/generate_load(646)
sendp(send_packet, iface="tap0")


