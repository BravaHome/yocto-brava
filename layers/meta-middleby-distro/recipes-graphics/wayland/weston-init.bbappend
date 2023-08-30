FILESEXTRAPATHS:prepend := "${THISDIR}/weston-init:"

SRC_URI += "file://weston-save-calibration.sh \
           "

do_install:append() {

    # Install calibration helper script and enable calibration
    install -Dm755 ${WORKDIR}/weston-save-calibration.sh ${D}${bindir}/weston-save-calibration.sh
    sed -i -e "/\[libinput\]/s/^#//g" ${D}${sysconfdir}/xdg/weston/weston.ini
    sed -i -e "/^\[libinput\]/a calibration_helper=/usr/bin/weston-save-calibration.sh" ${D}${sysconfdir}/xdg/weston/weston.ini
    sed -i -e "/^\[libinput\]/a touchscreen_calibrator=true" ${D}${sysconfdir}/xdg/weston/weston.ini

    # Configure Weston shell
    sed -i -e "/^#shell/d" ${D}${sysconfdir}/xdg/weston/weston.ini
    sed -i -e "/^\[core\]/a idle-time=0" ${D}${sysconfdir}/xdg/weston/weston.ini
    sed -i -e "/^\[core\]/a shell=kiosk-shell.so" ${D}${sysconfdir}/xdg/weston/weston.ini
}

FILES:${PN} += "${bindir}/weston-save-calibration.sh"
