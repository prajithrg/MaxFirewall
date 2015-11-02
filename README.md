## Simple Stateless Firewall 

![](https://github.com/prajithrg/MaxFirewall/blob/master/Firewall.jpg)

A simple firewall application using the Maxeler system which can be deployed transparently as a layer 2 bridge protecting a network. The application can be configured with INPUT and OUTPUT rule with respect to the network protected i.e. input rules are those rules which will be applied to the traffic flowing into the protected network and the output rules for traffic flowing out. As of now the application supports only a single rule, which is to drop a IP Protocol/Destination Port pair for input and output traffic.

## Building the Firewall App

### Bitstreams

Open the bitstream MaxCompiler Project in MaxIDE.

Open `FirewallManager.maxj` and Run as Java Application.

### Runtime

Copy the maxfile output from the above step to the `runtime` directory.

``` 
$ source config.sh
$ cd runtime
$ cp <pathToFirewallMaxFile> .
$ ./build.py
/opt/maxeler/maxcompiler/bin/sliccompile Firewall.max Firewall.o
Processing maxfile for MAX4AB24B_SIM from 'Firewall.max'.
gcc -std=gnu99 -Wall -Werror -fno-guess-branch-probability -frandom-seed=foo -Wno-unused-variable -Wno-unused-function -fPIC -I /opt/maxeler/maxcompiler/include/slic -DMAXFILE_INC="/home/demo/WS/git/MaxFirewall/runtime/Firewall.max" -DSLIC_NO_DESTRUCTORS -c /opt/maxeler/maxcompiler/src/slicinterface/MaxFileInit.c -o Firewall.o 
Copying .max file C object into '/home/demo/WS/git/MaxFirewall/runtime'
gcc -ggdb -O2 -fPIC -std=gnu99 -Wall -Werror -DDESIGN_NAME=Firewall -I. -I/opt/maxeler/maxcompiler/lib/maxeleros-sim/include -I/opt/maxeler/maxcompiler/include/slic -c firewallCpuCode.c -o firewallCpuCode.o
gcc firewallCpuCode.o -L/opt/maxeler/maxcompiler/lib -L/opt/maxeler/maxcompiler/lib/maxeleros-sim/lib -lslic -lmaxeleros -lm -lpthread Firewall.o -o firewall

To run in simulation, do:
	$ ./build.py run_sim
Then, in a new terminal:
	$ cd test
	$ sudo ./incomingSender.py
And in another terminal:
	$ sudo ./outgoingSender.py
```

The application will continue to wait for more packets, so when you are finished, hit <kbd>ctrl</kbd> + <kbd>c</kbd> to exit. The test infrastructure requires the `scapy` python module to be installed.


### Expected behaviour
The input and output firewall rules are configured in the `firewallCpuCode.c`. You can mention the IP protocol and destination port of the packet to be dropped. The packet drops will be printed on to stdout console as below with the frame number as dropped by the DFE.

```
...
...
Rules: INPUT--IPProtocol=17, DstPort=7653 :: OUTPUT-- IPProtocol=17, DstPort=1534
JDFE Running.
OUT Frame [1]: etherType = 0x800, ipProtocol = 17, dstPort = 1534 --> Dropped: 1
OUT Frame [3]: etherType = 0x800, ipProtocol = 17, dstPort = 1534 --> Dropped: 1
INPUT Frame [24]: etherType = 0x800, ipProtocol = 17, dstPort = 7653 --> Dropped: 1
INPUT Frame [26]: etherType = 0x800, ipProtocol = 17, dstPort = 7653 --> Dropped: 1
...
...
```

### Verifying output

This requires a packet sniffing tool like *wireshark* to read the dumped pcap file.

Verify that a network trace has been captured by running `ls` and looking for `TOP.pcap` and `BOT.pcap`.

```
$ ls
<...>
TOP.pcap
BOT.pcap
<...>
$ wireshark TOP.pcap &
$ wireshark BOT.pcap &
```

You can view only the packets of interest by filtering with the search terms..
