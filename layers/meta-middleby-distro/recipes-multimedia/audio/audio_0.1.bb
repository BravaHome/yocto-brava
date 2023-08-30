DESCRIPTION = "Audio Subsystem Setup"
SECTION = "applications"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit systemd

SRC_URI = "file://audio-0.1/audiosetup.sh \
           file://audio-0.1/audiosetup.service \
          "

SYSTEMD_SERVICE:${PN} = "audiosetup.service"
SYSTEMD_AUTO_ENABLE = "enable"

do_install:append () {
    install -d ${D}${bindir}
    install -m 0755 ${S}/audiosetup.sh ${D}${bindir}
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/audiosetup.service ${D}${systemd_system_unitdir}
}

RDEPENDS:${PN} += "alsa-utils-amixer"
