SUMMARY = "I2C Driver for EETI EGALAX Touchscreen Controller"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

inherit module

SRC_URI = "file://egalax_i2c.c \
           file://Makefile \
           file://egalax-i2c.conf \
          "

S = "${WORKDIR}"

do_install:append () {
    install -d ${D}${sysconfdir}/modules-load.d
    install -m 0755 ${S}/egalax-i2c.conf ${D}${sysconfdir}/modules-load.d
}

FILES:${PN} += "${sysconfdir}/modules-load.d/egalax-i2c.conf"