DESCRIPTION = "MLUI Hardware Test Application"
SECTION = "applications"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"


SRC_URI = "file://mlui-test.sh \
           file://serial-getty@ttyS0.service \
           file://Test-Pattern-5-1024x600-1b10fer.png \
           file://Test-Pattern-5-1280x768-1b10fer.png \
           file://Test-Pattern-5-800x480-1b10fer.png \
           file://modbus-register.h \
           file://modbus-server.c \
           file://modbus-client.c \
          "

BIN_DIR = "${ROOT_HOME}/mlui-test"
SYSTEMD_DIR = "${sysconfdir}/systemd/system"

S = "${WORKDIR}"

do_compile () {
    # quick and dirty
    ${CC} -lmodbus -o modbus-client modbus-client.c
    ${CC} -lmodbus -o modbus-server modbus-server.c
}

do_install () {
    install -d ${D}${BIN_DIR}
    install -m 0755 ${WORKDIR}/mlui-test.sh ${D}${BIN_DIR}
    install -m 0644 ${WORKDIR}/Test-Pattern-5-1024x600-1b10fer.png ${D}${BIN_DIR}
    install -m 0644 ${WORKDIR}/Test-Pattern-5-1280x768-1b10fer.png ${D}${BIN_DIR}
    install -m 0644 ${WORKDIR}/Test-Pattern-5-800x480-1b10fer.png ${D}${BIN_DIR}
    install -m 0755 ${WORKDIR}/modbus-client ${D}${BIN_DIR}
    install -m 0755 ${WORKDIR}/modbus-server ${D}${BIN_DIR}
    echo "/home/root/mlui-test/mlui-test.sh" > ${D}${ROOT_HOME}/.profile

    install -d ${D}${SYSTEMD_DIR}
    install -m 0644 ${WORKDIR}/serial-getty@ttyS0.service ${D}${SYSTEMD_DIR}
}

FILES:${PN} += "${ROOT_HOME} ${BIN_DIR} ${SYSTEMD_DIR}"

DEPENDS = "libmodbus"
RDEPENDS:${PN} += "fbida dialog"

INSANE_SKIP:${PN} = "ldflags"