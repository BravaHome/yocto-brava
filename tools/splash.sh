#!/bin/bash
IMAGE_JPG_INPUT=synesso-splash-800x464.jpg
IMAGE_UBOOT=synesso-uboot-splash-800x464.bmp
IMAGE_KERNEL=synesso-kernel-splash-800x464.ppm
echo "Converting $IMAGE_JPG_INPUT" 
jpegtopnm $IMAGE_JPG_INPUT | ppmquant 224 > tmp.ppm
pnmnoraw tmp.ppm > $IMAGE_KERNEL
echo ">> Kernel image created: $IMAGE_KERNEL" 
ppmtobmp -bpp 8 tmp.ppm > $IMAGE_UBOOT
echo ">> U-boot image created: $IMAGE_UBOOT" 
rm tmp.ppm
