DESCRIPTION = "Start MLUI Bluetooth Service"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit systemd

SRC_URI = "file://mlui-bluetooth.sh \
           file://mlui-bluetooth-high.sh \
           file://mlui-bluetooth.service \
          "

SYSTEMD_SERVICE:${PN} = "mlui-bluetooth.service"
SYSTEMD_AUTO_ENABLE = "enable"

do_install () {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/mlui-bluetooth.sh ${D}${bindir}
    install -m 0755 ${WORKDIR}/mlui-bluetooth-high.sh ${D}${bindir}
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/mlui-bluetooth.service ${D}${systemd_system_unitdir}
}

FILES:${PN} += "${bindir}/mlui-bluetooth.sh"

