#!/bin/sh

#
# Disable test for MLUI hardware on new production systems
#

SYSTEMCTL="/bin/systemctl"
RESULT_FILE="${HOME}/mlui-test/mlui-test.log"
GETTY_SERVICE="/etc/systemd/system/serial-getty@ttyS0.service"

MACHINE=`/bin/uname -n`

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
            break
            ;;

        "mlui-lcd7")
            ${SYSTEMCTL} ${act} virtuoso-ui >/dev/null 2>&1
            ${SYSTEMCTL} ${run} virtuoso-ui >/dev/null 2>&1
            ${SYSTEMCTL} ${act} virtuoso-r485d >/dev/null 2>&1
            ${SYSTEMCTL} ${run} virtuoso-r485d >/dev/null 2>&1
            break
            ;;

        "mlui-lcd7-usmp-t070")
            ${SYSTEMCTL} ${act} virtuoso-ui >/dev/null 2>&1
            ${SYSTEMCTL} ${run} virtuoso-ui >/dev/null 2>&1
            ${SYSTEMCTL} ${act} virtuoso-r485d >/dev/null 2>&1
            ${SYSTEMCTL} ${run} virtuoso-r485d >/dev/null 2>&1
            break
            ;;
    esac
    return 0
}


printf "Disabling Test Application "
printf "Remove .profile: "
rm ${HOME}/.profile >/dev/null 2>&1
printf "Remove GETTY Service: "
rm ${GETTY_SERVICE} >/dev/null 2>&1
printf "Reload systemd: " 
${SYSTEMCTL} daemon-reload
printf "Enable Application Service: "
enable_application_service 1


