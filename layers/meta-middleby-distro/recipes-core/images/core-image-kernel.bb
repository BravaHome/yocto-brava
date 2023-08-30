SUMMARY = "Image for Kernel and Device Tree"
DESCRIPTION = "Used for RAUC updates of kernel partition"
LICENSE = "MIT"

inherit image

DEPENDS += "bash"
IMAGE_CLASSES = ""
IMAGE_FSTYPES = "ext4"

IMAGE_INSTALL = "kernel-image kernel-devicetree"


massage_rootfs() {
    mkdir -p ${WORKDIR}/stage
    mv ${IMAGE_ROOTFS}/boot ${WORKDIR}/stage
    rm -r ${IMAGE_ROOTFS}/*
    mv ${WORKDIR}/stage/boot/* ${IMAGE_ROOTFS}/
    mkdir -p ${IMAGE_ROOTFS}/allwinner
    mv ${IMAGE_ROOTFS}/*.dtb ${IMAGE_ROOTFS}/allwinner
}
IMAGE_PREPROCESS_COMMAND:append = " massage_rootfs;"
