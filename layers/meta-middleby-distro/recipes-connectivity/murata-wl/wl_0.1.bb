DESCRIPTION = "Murata test tool for wireless chipsets"
SECTION = "applications"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"


inherit bin_package


SRC_URI = "file://wl \
          "

do_install () {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/wl ${D}${bindir}
}

#FILES:${PN} += "${VIRTUOSO_INSTALL_DIR}"

RDEPENDS:wl += "libnl libnl-genl libnl-nf libnl-route"
INSANE_SKIP:${PN} += "ldflags"
