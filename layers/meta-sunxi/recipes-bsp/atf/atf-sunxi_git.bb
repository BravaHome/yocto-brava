DESCRIPTION = "ARM Trusted Firmware Allwinner"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://license.md;md5=829bdeb34c1d9044f393d5a16c068371"
LIC_FILES_CHKSUM:sun50i-h616 = "file://docs/license.rst;md5=b2c740efedc159745b9b31f88ff03dde"

SRC_URI = " \
git://github.com/apritzel/arm-trusted-firmware;nobranch=1;protocol=https \
file://0001-Use-same-type-as-in-declaration.patch \
"
SRCREV = "aa75c8da415158a94b82a430b2b40000778e851f"

SRC_URI:sun50i-h616 = "git://github.com/ARM-software/arm-trusted-firmware;nobranch=1;protocol=https"
SRCREV:sun50i-h616 = "f04dfbb297f03d7f8d7f7c00ce8712e1a10295cf"

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

COMPATIBLE_MACHINE = "(sun50i|sun50i-h616)"

PLATFORM:sun50i = "sun50iw1p1"
PLATFORM:sun50i-h616 = "sun50i_h616"

LDFLAGS[unexport] = "1"

do_compile() {
    oe_runmake -C ${S} BUILD_BASE=${B} \
      CROSS_COMPILE=${TARGET_PREFIX} \
      PLAT=${PLATFORM} \
      bl31 \
      all
}

do_install() {
    install -D -p -m 0644 ${B}/${PLATFORM}/release/bl31.bin ${DEPLOY_DIR_IMAGE}/bl31.bin
}
