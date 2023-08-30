FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://Makefile.dts \
            file://sun50i-a64-mlui.dts \
            file://a64-mlui_defconfig \
            file://fw_env.config \
            file://${BOOTSCRIPT} \
           "

BOOTSCRIPT = "${@bb.utils.contains('DISTRO_FEATURES', 'rauc', 'boot-mlui-${POST_CONFIGURATION}.cmd.rauc', 'boot-mlui-${POST_CONFIGURATION}.cmd', d)}"

do_configure:prepend() {
    cp ${WORKDIR}/a64-mlui_defconfig ${S}/configs
    cp ${WORKDIR}/sun50i-a64-mlui.dts ${S}/arch/arm/dts
    cp ${WORKDIR}/Makefile.dts ${S}/arch/arm/dts/Makefile
    cp ${WORKDIR}/${BOOTSCRIPT} ${WORKDIR}/boot.cmd
}

do_deploy:append() {
    install -d ${DEPLOYDIR}
    install -m 644 ${B}/u-boot-sunxi-with-spl.bin ${DEPLOYDIR}
}
