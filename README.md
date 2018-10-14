# Real-time-Tasks-Models-in-Linux
Team Members :
1. Bhavana Santhoshi Siga - 1214945459
2. Sanchari Datta - 1215306118


Files :
Source code   : newcode.c;
Text file     : input;
Output file   : outputfile.txt;
Makefile      : Makefile;
Report        : report.pdf;

Prerequisites:
Linux kernel (preferably linux 2.6.19 and above) GNU (preferably gcc 4.5.0 and above)

Obtaining the mouse event : We used the event "mice" to detect the mouse clicks and activate the aperiodic tasks. Otherwise, using cat /proc/bus/input/devices, we can find the mouse event number which will vary for each host.

Galileo Set up : The Boot Image is flashed into an SD card and inserted into the Board. A Static IP address is set to the board and a connection is established between the host machine and Galileo. Then the toolchain SDK is downloaded and extracted into a directory on your host machine

Connection between host and Galileo : We can either use the shell screen to establish communication between the host machine and the Galileo or use Putty. Find the ip address of the  ethernet connection and the Galileo, which can be set according to our wish by using the command ifconfig  enp0s20f6 192.168.1.5  netmask  255.255.0.0  up 

The SDK is extracted to the directory : /opt/iot-devkit/1.7.2/sysroots

The code is edited,compiled and run using GCC compiler using the commands gedit(editing), gcc(compiling), ./a.out(running).

Installing GCC compiler 

sudo apt-get install gcc - this would install gcc compiler on hostmachine

The makefile is compiled on the host machine using make by commenting the appropriate lines (CC=/home/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/i586-poky-linux-gcc
SROOT=/home/SDK/sysroots/i586-poky-linux/

The makefile is compiled on the Galileo using the command "make" by commenting the appropriate lines (CC = gcc).

The code can be found in newcode.c wherein the code lines are commented to give a description of the functions used.

When you run .out file,we can see the periodic threads getting activated and executing the task body. The threads, along with their numbers are written into a file called outputfile.txt. When we click the left or right mouse button, aperiodic threads get activated. 

Use kernelshark trace.dat -- which would display the threads schedule times.
  
