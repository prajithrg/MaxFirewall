## A Simple transparent Layer 2 Firewall with IP Protocol/Destination Port In/Output Rules 

![](https://github.com/prajithrg/MaxFirewall/blob/master/Firewall.jpg)

A simple firewall application which acts as a transparent layer 2 bridge protecting a network. The application can be configured with INPUT and OUTPUT rule with respect to the network protected i.e. input rules are those rules which will be applied to the traffic flowing into the protected network and the output rules for traffic flowing out. As of now the application supports only a single rule, which is a IP Protocol/Destination Port pair for input and output traffic.

## Building the Firewall App

### Bitstreams

Open the bitstream MaxCompiler Project in MaxIDE.

Open `FirewallManager.maxj` and Run as Java Application.

### Runtime

Copy the maxfile output from the above step to the `runtime` directory.

```
$ source config.sh
$ cd runtime
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


The application will continue to wait for more packets, so when you are finished, hit <kbd>ctrl</kbd> + <kbd>c</kbd> to exit.


## Expected behaviour


## Verifying output

This requires a packet sniffing tool like *wireshark* to read the dumped pcap file.

Verify that a network trace has been captured by running `ls` and looking for `TOP.pcap` and `BOT.pcap`.

```
$ ls
<...>
TOP.pcap
<...>
$ wireshark TOP.pcap &
```

You can view only the packets of interest by filtering with the search terms..
