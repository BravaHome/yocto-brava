SUMMARY = "Driver for Realtek RTL88x2BU WiFi/BT Controllers"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

inherit module

SRC_URI = " git://github.com/cilynx/rtl88x2bu.git;protocol=https;branch=5.8.7.1_35809.20191129_COEX20191120-7777 \
          "
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

EXTRA_OEMAKE += "KSRC=${STAGING_KERNEL_DIR} ARCH=arm64 MODDESTDIR=${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}"

MODULE_NAME = "88x2bu.ko"
MODULE_DIR = "${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/wireless/realtek"

do_install () {
    install -d ${MODULE_DIR}
    install -p -m 644 ${S}/${MODULE_NAME} ${MODULE_DIR}/rtl${MODULE_NAME}
}

#FILES:${PN} += "${sysconfdir}/modules-load.d/egalax-i2c.conf"
