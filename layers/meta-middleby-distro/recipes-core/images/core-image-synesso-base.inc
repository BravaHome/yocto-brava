SUMMARY = "Base image for Synesso systems based on MLUI"
DESCRIPTION = "Image for the Synesso configuration building on top of the MLUI base image. \
               This file is meant to be included by the actual image recipe. \
              "

LICENSE = "MIT"

require core-image-mlui-base.inc

# Add to system packages
IMAGE_INSTALL_SYSTEM += "${IMAGE_INSTALL_GRAPHICS} ${IMAGE_INSTALL_SYNESSO}"

# Graphics Packages
IMAGE_INSTALL_GRAPHICS = "\
	weston \
	weston-init \
	weston-examples \
	"

# Software Packages for the base image
IMAGE_INSTALL_SYNESSO = "\
    util-linux \
	synesso-single-group \
"

IMAGE_ROOTFS_EXTRA_SPACE:append=" + 2097152"

