FILESEXTRAPATHS:prepend := "${THISDIR}/linux-mainline:"

SRC_URI += "file://sun50i-a64-${MACHINE}.dts \
            file://sun50i-a64-mlui.dtsi \
            file://mlui_defconfig \
            file://middleby-kernel-splash-1024x592.ppm \
            file://middleby-kernel-splash-1280x768.ppm \
            file://middleby-kernel-splash-800x464.ppm \
            file://synesso-kernel-splash-800x464.ppm \
            file://0001-Enabling-interrupts-for-PCA957x.patch \
"

KERNEL_SPLASH ??= "middleby-kernel-splash-1024x592.ppm"

do_configure:prepend() {
    cp ${WORKDIR}/sun50i-a64-${MACHINE}.dts ${S}/arch/${ARCH}/boot/dts/allwinner/sun50i-a64-mlui.dts
    cp ${WORKDIR}/sun50i-a64-mlui.dtsi ${S}/arch/${ARCH}/boot/dts/allwinner/sun50i-a64-mlui.dtsi
    echo -e "dtb-\x24(CONFIG_ARCH_SUNXI) += sun50i-a64-mlui.dtb" >> ${S}/arch/${ARCH}/boot/dts/allwinner/Makefile

    if [ -e ${WORKDIR}/${KERNEL_SPLASH} ]; then
        cp ${WORKDIR}/${KERNEL_SPLASH} ${S}/drivers/video/logo/logo_linux_clut224.ppm
        kernel_conf_variable LOGO_LINUX_CLUT224 y
    fi
}

