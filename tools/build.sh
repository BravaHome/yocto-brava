#!/bin/bash
# (c) 2021, ibeeto, Rudolf J Streif
# Build script for Middleby MLUI project 

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
POSTCONF=""

SCRIPT_PATH="$( cd "$(dirname ${0})" >/dev/null 2>&1 ; pwd -P )"
PROJECT_PATH="$(dirname ${SCRIPT_PATH})"
SDK_PATH="${PROJECT_PATH}/build/deploy/sdk"
POKY_PATH="${PROJECT_PATH}/layers/poky"
BUILD_ENV="build"

select_machine () {
   PS3="Please choose the machine to build for: "
   select MACHINE in "${MACHINES[@]}"
   do
      [ $REPLY -ge 1 -a $REPLY -le ${#MACHINES[@]} 2>/dev/null ] && break
      printf "Invalid option.\n"
   done
}

select_image () {
   PS3="Please choose the image to build: "
   select IMAGE in "${IMAGES[@]}"
   do
      [ $REPLY -ge 1 -a $REPLY -le ${#IMAGES[@]} 2>/dev/null ] && break
      printf "Invalid option.\n"
   done
}

select_image_type () {
   PS3="Please choose the image type to build: "
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
            IMAGETYPE="-sdk"
            break
	    ;;
	 *)
            printf "Invalid option.\n"
	    ;;
      esac
   done
}


printf "\n\n"
printf "BUILDING DEVICE IMAGE\n"
printf "=====================\n\n"

select_machine
printf "\n"
select_image
printf "\n"
select_image_type
printf "\n"

# Setup build environment
pushd ${PROJECT_PATH} >/dev/null 2>/dev/null
source ${POKY_PATH}/oe-init-build-env ${BUILD_ENV}

# Set post configuration file
POSTCONF=`echo ${IMAGE} | cut -d '-' -f 3`
POSTCONF="conf/${POSTCONF}.conf"

# SDK is created for the development image
if [ "${IMAGETYPE}" == "-sdk" ]; then
   MACHINE="${MACHINE}" bitbake -R ${POSTCONF} -k ${IMAGE}-dev
   MACHINE="${MACHINE}" bitbake -R ${POSTCONF} -k ${IMAGE}-dev -c populate_sdk
else
   MACHINE="${MACHINE}" bitbake -R ${POSTCONF} -k ${IMAGE}${IMAGETYPE}
fi

popd >/dev/null 2>/dev/null

# Building complete
printf "SUCCESS: Bulding complete.\n"
printf "\n\n"

exit 0
