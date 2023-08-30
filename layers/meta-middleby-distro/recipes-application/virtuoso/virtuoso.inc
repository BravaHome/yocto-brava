DESCRIPTION = "Virtuoso Oven Application"
SECTION = "applications"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"


inherit systemd
inherit bin_package


SRC_URI = "file://r485d \
           file://r485c \
           file://virtuoso \
           file://${VIRTUOSO_SERVICE} \
           file://virtuoso-r485d.service \
           file://install.py \
           file://simple_ota.py \
           file://kos-appliance.conf \
          "

SYSTEMD_SERVICE:${PN} = "virtuoso-r485d.service virtuoso-ui.service"
SYSTEMD_AUTO_ENABLE = "enable"

VIRTUOSO_SERVICE:mlui-lcd7 = "virtuoso-ui.service.mlui-lcd7"
VIRTUOSO_SERVICE:mlui-lcd7-usmp-t070 = "virtuoso-ui.service.mlui-lcd7-usmp-t070"
VIRTUOSO_INSTALL_DIR = "/opt/virtuoso"

do_install () {
    install -d ${D}${VIRTUOSO_INSTALL_DIR}
    install -m 0755 ${WORKDIR}/r485d ${D}${VIRTUOSO_INSTALL_DIR}
    install -m 0755 ${WORKDIR}/r485c ${D}${VIRTUOSO_INSTALL_DIR}
    install -m 0755 ${WORKDIR}/virtuoso ${D}${VIRTUOSO_INSTALL_DIR}
    install -m 0755 ${WORKDIR}/install.py ${D}${VIRTUOSO_INSTALL_DIR}
    install -m 0755 ${WORKDIR}/simple_ota.py ${D}${VIRTUOSO_INSTALL_DIR}
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/${VIRTUOSO_SERVICE} ${D}${systemd_system_unitdir}/virtuoso-ui.service
    install -m 0644 ${WORKDIR}/virtuoso-r485d.service ${D}${systemd_system_unitdir}/virtuoso-r485d.service
    install -d ${D}${sysconfdir}/dbus-1/system.d
    install -m 0644 ${WORKDIR}/kos-appliance.conf ${D}${sysconfdir}/dbus-1/system.d/kos-applance.conf
}

FILES:${PN} += "${VIRTUOSO_INSTALL_DIR} \
               "
#DEPENDS += "qtbase qtdeclarative qtconnectivity qtserialport libgles2-mesa libgpiod libatomic"
RDEPENDS:${PN} += "qtbase qtdeclarative qtconnectivity qtserialport qtmultimedia libgles2-mesa libgpiod libatomic"
INSANE_SKIP:${PN} += "ldflags"
