## A Simple transparent Layer 2 Firewall with IP Protocol/Destination Port In/Output Rules 

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
$ cp <pathToMaxFile> .
$ ./build.py
/network-raid/opt/maxcompiler-2014.1.1/bin/sliccompile SignExt.max SignExt.o
Processing maxfile for MAX4AB24B_SIM from 'SignExt.max'.
gcc -std=gnu99 -Wall -Werror -fno-guess-branch-probability -frandom-seed=foo -Wno-unused-variable -Wno-unused-function -fPIC -I /network-raid/opt/maxcompiler-2014.1.1/include/slic -DMAXFILE_INC="/home/mhaslehurst/Workspaces/networking-examples/PacketProcessing/SignExtender/hostcode/SignExt.max" -DSLIC_NO_DESTRUCTORS -c /network-raid/opt/maxcompiler-2014.1.1/src/slicinterface/MaxFileInit.c -o SignExt.o 
Copying .max file C object into '/home/mhaslehurst/Workspaces/networking-examples/PacketProcessing/SignExtender/hostcode'
gcc -ggdb -O2 -fPIC -std=gnu99 -Wall -Werror -DDESIGN_NAME=SignExt -I. -I/network-raid/opt/maxcompiler-2014.1.1/lib/maxeleros-sim/include -I/network-raid/opt/maxcompiler-2014.1.1/include/slic -c signext.c -o signext.o
gcc signext.o -L/network-raid/opt/maxcompiler-2014.1.1/lib -L/network-raid/opt/maxcompiler-2014.1.1/lib/maxeleros-sim/lib -lslic -lmaxeleros -lm -lpthread SignExt.o -o signext
$ cd ..
```

## Expected behaviour

The following two scenarios are used in this example (values set in `sender.c`).

* Packet 1:

First byte : 0x43

Variable | Size | Value | Expected output to CPU
:-------:|:----:|:-----:|:---------------------:
A | 3 | 0x800201 | 0xffffffffff800201
B | 0 | 0x7766554433221100UL | 0x7766554433221100
C | 1 | 0x80 | 0xffffff80

* Packet 2:

First byte : 0xCE

Variable | Size | Value | Expected output to CPU
:-------:|:----:|:-----:|:---------------------:
A | 6 | 0x800102030405UL | 0xffff800102030405
B | 1 | 0x78 | 0x78
C | 3 | 0x818283 | 0xff818283

## Building

### Bitstreams

Open the bitstream Java Project.

Open `SignExtManager.maxj` and execute the `main()` function.

### Runtime

Copy the maxfile output from the Bitstream to the `runtime` directory.

```
$ source config.sh
$ cd runtime
$ ./build.py
/network-raid/opt/maxcompiler-2014.1.1/bin/sliccompile SignExt.max SignExt.o
Processing maxfile for MAX4AB24B_SIM from 'SignExt.max'.
gcc -std=gnu99 -Wall -Werror -fno-guess-branch-probability -frandom-seed=foo -Wno-unused-variable -Wno-unused-function -fPIC -I /network-raid/opt/maxcompiler-2014.1.1/include/slic -DMAXFILE_INC="/home/mhaslehurst/Workspaces/networking-examples/PacketProcessing/SignExtender/hostcode/SignExt.max" -DSLIC_NO_DESTRUCTORS -c /network-raid/opt/maxcompiler-2014.1.1/src/slicinterface/MaxFileInit.c -o SignExt.o 
Copying .max file C object into '/home/mhaslehurst/Workspaces/networking-examples/PacketProcessing/SignExtender/hostcode'
gcc -ggdb -O2 -fPIC -std=gnu99 -Wall -Werror -DDESIGN_NAME=SignExt -I. -I/network-raid/opt/maxcompiler-2014.1.1/lib/maxeleros-sim/include -I/network-raid/opt/maxcompiler-2014.1.1/include/slic -c signext.c -o signext.o
gcc signext.o -L/network-raid/opt/maxcompiler-2014.1.1/lib -L/network-raid/opt/maxcompiler-2014.1.1/lib/maxeleros-sim/lib -lslic -lmaxeleros -lm -lpthread SignExt.o -o signext
$ cd ..
```

### Packet sender

Build the auxiliary application, used to send the packets.

```
$ cd sender
$ ./build.py
gcc -ggdb -O2 -fPIC -std=gnu99 -Wall -Werror -c sender.c -o sender.o
gcc sender.o -o sender
$ cd ..
```

## Running

```
$ cd runtime
$ ./run.py
<...>
Listening on 172.16.50.1 port 2000
Waiting for kernel response...
```

Then open a new terminal and inject some packets:

```
$ ./sender/sender
Sender finished
```

In the first terminal, you should see the received packet details:

```
Kernel got: aSz = 3, bSz = 0, cSz = 1
Kernel got: aSz = 6, bSz = 1, cSz = 3
CPU: Got output frame 1 - size 20 bytes
Frame [1] Word[0]: 0xffffffffff800201
Frame [1] Word[1]: 0x7766554433221100
Frame [1] Word[2]: 0xffffff80
CPU: Got output frame 2 - size 20 bytes
Frame [2] Word[0]: 0xffff800102030405
Frame [2] Word[1]: 0x78
Frame [2] Word[2]: 0xff818283
```

The application will continue to wait for more packets, so when you are finished, hit <kbd>ctrl</kbd> + <kbd>c</kbd> to exit.

## Verifying output

This requires that *wireshark* is installed.

Verify that a network trace has been captured by running `ls` and looking for `top1.pcap`.

```
$ ls
<...>
top1.pcap
<...>
$ wireshark top1.pcap &
```

You can view only the packets of interest by filtering with the search term `ip.dst==172.16.50.1`.
