#make file
CC=/opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/i586-poky-linux-gcc
SROOT=/home/SDK/sysroots/i586-poky-linux/
#CC = gcc 

CFLAGS = -g -Wall -pthread -std=c99

TARGET = newcode

all : $(TARGET).c

	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:

	$(RM) $(TARGET)




