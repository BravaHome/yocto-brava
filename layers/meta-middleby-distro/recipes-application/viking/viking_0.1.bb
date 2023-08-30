DESCRIPTION = "Viking MLUI Startup"
SECTION = "applications"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"


inherit systemd

DEPENDS += "base-files"

SRC_URI = "file://startup_stage1.sh \
           file://mluistart.service \
          "

SYSTEMD_SERVICE:${PN} = "mluistart.service"
SYSTEMD_AUTO_ENABLE = "enable"

do_install () {
    install -d ${D}${ROOT_HOME}
    install -m 0755 ${WORKDIR}/startup_stage1.sh ${D}${ROOT_HOME}
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/mluistart.service ${D}${systemd_system_unitdir}
}

FILES:${PN} += "${ROOT_HOME}/startup_stage1.sh"
