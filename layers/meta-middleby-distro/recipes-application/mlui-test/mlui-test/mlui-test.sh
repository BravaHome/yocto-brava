#!/bin/sh

#
# Test script for MLUI hardware
#

DIALOG="/usr/bin/dialog --ascii-lines"
SYSTEMCTL="/bin/systemctl"
FBIDA="/usr/bin/fbi --noverbose --nointeractive"
DD="/bin/dd"
GETTY_SERVICE="/etc/systemd/system/serial-getty@ttyS0.service"

MACHINE=`/bin/uname -n`
MACHINES=("mlui-lcd5" "mlui-lcd7" "mlui-lcd7-usmp-t070")

DRIVE_DEVICE="/dev/sda1"
DRIVE_MPOINT="/media/sda1"

IMAGE=""
STATUS="OK"
RETRIES=3

RESULT_FILE="/home/root/mlui-test/mlui-test.log"
MODBUS_SERVER="/home/root/mlui-test/modbus-server -c /dev/ttyS2"
MODBUS_CLIENT="/home/root/mlui-test/modbus-client -c /dev/ttyS3"

NOW=$(/bin/date "+%Y.%m.%d-%H.%M.%S")

test_result () {
    if [ $1 -eq 0 ]; then
        printf "OK\n" >> ${RESULT_FILE}
    else
        printf "FAIL\n" >> ${RESULT_FILE}
	STATUS="FAIL"
    fi
}

enable_application_service () {
    if [ $1 -eq 1 ]; then
        run="start"
        act="enable"
    else
        run="stop"
        act="disable"
    fi
    case ${MACHINE} in
        "mlui-lcd5")
            ${SYSTEMCTL} ${act} mdb_ui >/dev/null 2>&1
            ${SYSTEMCTL} ${run} mdb_ui >/dev/null 2>&1
	        IMAGE="/home/root/mlui-test/Test-Pattern-5-800x480-1b10fer.png"
            break
            ;;

        "mlui-lcd7")
            ${SYSTEMCTL} ${act} virtuoso-ui >/dev/null 2>&1
            ${SYSTEMCTL} ${run} virtuoso-ui >/dev/null 2>&1
            ${SYSTEMCTL} ${act} virtuoso-r485d >/dev/null 2>&1
            ${SYSTEMCTL} ${run} virtuoso-r485d >/dev/null 2>&1
	        IMAGE="/home/root/mlui-test/Test-Pattern-5-1024x600-1b10fer.png"
            break
	    ;;

        "mlui-lcd7-usmp-t070")
            ${SYSTEMCTL} ${act} virtuoso-ui >/dev/null 2>&1
            ${SYSTEMCTL} ${run} virtuoso-ui >/dev/null 2>&1
            ${SYSTEMCTL} ${act} virtuoso-r485d >/dev/null 2>&1
            ${SYSTEMCTL} ${run} virtuoso-r485d >/dev/null 2>&1
	        IMAGE="/home/root/mlui-test/Test-Pattern-5-1280x768-1b10fer.png"
            break
            ;;
    esac
    return 0
}

test_usb_drive () {
    printf "Drive Mount: " >> ${RESULT_FILE}
    loop=0
    while [ ${loop} -le ${RETRIES} ];
    do
        ${DIALOG} --title "MLUI TEST" --msgbox "Insert USB drive into USB socket and wait 30 seconds.\n\nPress ENTER to confirm." 10 50
        grep -qs ${DRIVE_DEVICE} /proc/mounts
        if [ $? -eq 0 ]; then
            break
        fi
        loop=$(( $loop + 1 ))
    done
    if [ ${loop} -le ${RETRIES} ]; then
        test_result  0
        ${DIALOG} --title "MLUI TEST" --infobox "Please wait until USB drive test has completed." 10 50
        printf "Drive Test: " >> ${RESULT_FILE}
        mpoint=`grep -s ${DRIVE_DEVICE} /proc/mounts | cut -d " " -f 2`
        ${DD} of=/tmp/mlui-test.file if=/dev/urandom bs=1M count=10 >/dev/null 2>&1
        cp /tmp/mlui-test.file ${mpoint} >/dev/null 2>&1
        sync
        cmp /tmp/mlui-test.file ${mpoint}/mlui-test.file >/dev/null 2>&1
        test_result $?
        printf "Drive Unmount: " >> ${RESULT_FILE}
        loop=0
        while [ ${loop} -le ${RETRIES} ];
        do
            grep -qs ${DRIVE_DEVICE} /proc/mounts
            if [ $? -eq 1 ]; then
                break
            fi
            ${DIALOG} --title "MLUI TEST" --msgbox "Remove USB drive from USB socket.\n\nPress ENTER to confirm." 10 50
            loop=$(( $loop + 1 ))
        done
        if [ ${loop} -le ${RETRIES} ]; then
            test_result  0
        else
            test_result  1
        fi
    else
        test_result 1
    fi
}

test_modbus () {
    ${DIALOG} --title "MLUI TEST" --msgbox "Plug in RS485 test cable.\n\nPress ENTER to confirm." 10 50
    ${DIALOG} --title "MLUI TEST" --infobox "Please wait until RS485 test has completed." 10 50
    printf "Start Modbus Server: " >> ${RESULT_FILE}
    ${MODBUS_SERVER} >> ${RESULT_FILE} 2>&1 &
    test_result $?
    printf "Start Modbus Client: \n" >> ${RESULT_FILE}
    ${MODBUS_CLIENT} >> ${RESULT_FILE} 2>&1
    test_result $?
    /usr/bin/killall modbus-server
    ${DIALOG} --title "MLUI TEST" --msgbox "Unplug RS485 test cable.\n\nPress ENTER to confirm." 10 50
}

dmesg -D
clear

printf "MLUI TEST\n"

printf "TIME: ${NOW}"
printf "MLUI: ${MACHINE}\n"

printf "[${NOW}]\n" >> ${RESULT_FILE}
printf "MACHINE: ${MACHINE}\n" >> ${RESULT_FILE}

printf "Disable Application Service: " >> ${RESULT_FILE}
enable_application_service 0
test_result $?

printf "Console: " >> ${RESULT_FILE}
clear
${DIALOG} --title "MLUI TEST" --msgbox "If you see this message, press ENTER to confirm." 10 50
test_result $?

# Display Test
${FBIDA} ${IMAGE} >/dev/null 2>/dev/null &

printf "TP Show: " >> ${RESULT_FILE}
clear
${DIALOG} --title "MLUI TEST" --yesno "Do you see the test pattern on the display?" 10 50
test_result $?

printf "TP Aspect Ratio: " >> ${RESULT_FILE}
clear
${DIALOG} --title "MLUI TEST" --yesno "Are the four white circles in the corners round?" 10 50
test_result $?

printf "TP Color: " >> ${RESULT_FILE}
clear
${DIALOG} --title "MLUI TEST" --yesno "Do you see colors on the screen?" 10 50
test_result $?

printf "TP Greyscale: " >> ${RESULT_FILE}
clear
${DIALOG} --title "MLUI TEST" --yesno "Do you see the grey boxes from 10% to 100%?" 10 50
test_result $?

/usr/bin/killall fbi >/dev/null 2>&1


# USB Drive Test
test_usb_drive

# Modbus Test
test_modbus

# Cleanup
${DIALOG} --title "MLUI TEST" --infobox "Analyzing test results...\n\nPlease stand by..." 10 50
if [ ${STATUS} = "OK" ]; then
    printf "Remove .profile: " >> ${RESULT_FILE}
    rm ${HOME}/.profile >/dev/null 2>&1
    test_result $?
    printf "Remove GETTY Service: " >> ${RESULT_FILE}
    rm ${GETTY_SERVICE} >/dev/null 2>&1
    test_result $?
    printf "Reload systemd: " >> ${RESULT_FILE}
    ${SYSTEMCTL} daemon-reload
    test_result $?
    printf "Enable Application Service: " >> ${RESULT_FILE}
    enable_application_service 1
    test_result $?
    ${DIALOG} --title "MLUI TEST" --colors --pause "\Zb\Z2TESTS COMPLETED SUCCESSFULLY.\Zn\n\nShutdown in 5s.\n\nWait for 'power down' message before removing power." 15 50 5
else
    printf "Enable Application Service: " >> ${RESULT_FILE}
    enable_application_service 1
    test_result $?
    ${DIALOG} --title "MLUI TEST" --colors --pause "\Zb\Z1TESTS FAILED.\Zn\n\nShutdown in 5s.\n\nWait for 'power down' message before removing power." 15 50 5
fi

clear
printf "STATUS: ${STATUS}\n\n" >> ${RESULT_FILE}

${SYSTEMCTL} poweroff