#!/bin/sh

mkdir /mnt/thumb

mount /dev/sda1 /mnt/thumb

cp /mnt/thumb/startup_stage2.sh /home/root/startup_stage2.sh

chmod 0777 /home/root/startup_stage2.sh

/home/root/startup_stage2.sh
