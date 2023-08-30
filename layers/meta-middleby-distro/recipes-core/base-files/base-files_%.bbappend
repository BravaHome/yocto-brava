FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://fstab.${POST_CONFIGURATION}"

do_configure:prepend() {
    cp ${WORKDIR}/fstab.${POST_CONFIGURATION} ${WORKDIR}/fstab
}
