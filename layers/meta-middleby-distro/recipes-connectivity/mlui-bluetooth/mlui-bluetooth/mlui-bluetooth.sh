#!/bin/sh
#
# Attach to EA 1MW Bluetooth Module
#

PORT="/dev/ttyS1"
SPEED="115200"

/usr/bin/hciattach ${PORT} bcm43xx ${SPEED} flow && /usr/bin/hciconfig hci0 up && exit 1
exit 0

