#!/bin/sh
#
# Switch EA 1MW Bluetooth Module to high speed UART
#

PORT="/dev/ttyS1"
HCIDEV="hci0"
LOWSPEED="115200"
HIGHSPEED="3000000"


/bin/echo "Shutting down: ${HCIDEV}"
/usr/bin/hciconfig ${HCIDEV} down
killall hciattach
/bin/echo "Attaching with ${LOWSPEED}"
/usr/bin/hciattach ${PORT} any -s ${LOWSPEED} ${LOWSPEED} flow
/usr/bin/hciconfig ${HCIDEV} up
/bin/echo "Setting to ${HIGHSPEED}"
/usr/bin/hcitool -i ${HCIDEV} cmd 0x3f 0x0009 0xc0 0xc6 0x2d 0x00
/usr/bin/hciconfig ${HCIDEV} down
killall hciattach
/bin/echo "Attaching with ${HIGHSPEED}"
/usr/bin/hciattach ${PORT} any -s ${HIGHSPEED} ${HIGHSPEED} flow
/usr/bin/hciconfig ${HCIDEV} up

