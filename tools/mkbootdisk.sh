#!/bin/bash
# (c) 2020, ibeeto, Rudolf J Streif
# Create bootable disk for Middleby BSP
# Works for USB drives and SD cards

# Variables Section

# List of target machines
MACHINES=("mlui-lcd7" "mlui-lcd5" "mlui-lcd7-usmp-t070")

# List of target images
IMAGES=("core-image-mlui" "core-image-viking" "core-image-synesso")

# List of target image types
IMAGETYPES=("production" "development" "sdk")


# Code Section

MACHINE=""
IMAGE=""
IMAGETYPE=""

SCRIPT_PATH="$( cd "$(dirname ${0})" >/dev/null 2>&1 ; pwd -P )"
PROJECT_PATH="$(dirname ${SCRIPT_PATH})"

select_machine () {
   PS3="Please choose the machine to flash: "
   select MACHINE in "${MACHINES[@]}"
   do
      [ $REPLY -ge 1 -a $REPLY -le ${#MACHINES[@]} 2>/dev/null ] && break
      printf "Invalid option.\n"
   done
}

select_image () {
   PS3="Please choose the image to flash: "
   select IMAGE in "${IMAGES[@]}"
   do
      [ $REPLY -ge 1 -a $REPLY -le ${#IMAGES[@]} 2>/dev/null ] && break
      printf "Invalid option.\n"
   done
}

select_image_type () {
   PS3="Please choose the image type to flash: "
   select IMAGETYPE in "${IMAGETYPES[@]}"
   do
      case $IMAGETYPE in
         "production")
            IMAGETYPE="-prd"
	    break
	    ;;
	 "development")
            IMAGETYPE="-dev"
            break
	    ;;
	 "sdk")
            IMAGETYPE="-dev"
            break
	    ;;
	 *)
            printf "Invalid option.\n"
	    ;;
      esac
   done
}


printf "\n\n"
printf "CREATING BOOTABLE DISK\n"
printf "======================\n\n"

test "$(id -u)" != "0" && echo "You need to be 'root' to run this script." && exit 1

select_machine
printf "\n"
select_image
printf "\n"
select_image_type

DEPLOY_PATH="${PROJECT_PATH}/build/deploy/images/${MACHINE}"
DISK_IMAGE="${DEPLOY_PATH}/${IMAGE}${IMAGETYPE}-${MACHINE}.wic"

# Check for image files
test ! -f ${DISK_IMAGE} && echo "Disk image file not found. Do you need to build first?" && exit 1

printf "\n"
printf "Flashing image: \n"
printf "${DISK_IMAGE}\n"

# Enter device name
printf "\n"
printf "Insert drive and enter device name: "
read DEVICE
test -z "${DEVICE}" && echo "No disk device specified." && exit 1
test "${DEVICE}" = "/dev/sda" && echo "OOPS - System disk specified." && exit 1

# Verify
printf "\n"
printf "Creating a bootable disk will erase all data on ${DEVICE}.\n"
printf "Are you sure that you want to proceed? Type YES to proceed: "
read RESPONSE
test "$RESPONSE" != "YES" && echo "Exiting." && exit 1

# Unmount all partitions
umount ${DEVICE}* >/dev/null 2>/dev/null

# Flash the device
sudo dd if=${DISK_IMAGE} of=${DEVICE} bs=1M  iflag=fullblock oflag=direct conv=fsync status=progress


printf "Complete.\n\n".
