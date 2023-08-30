SUMMARY = "Image for MLUI Development System"
DESCRIPTION = "Image for the MLUI development system configuration building on top of the base image."

LICENSE = "MIT"

require core-image-mlui-base.inc

# System Software Packages
IMAGE_INSTALL_SYSTEM += "\
        packagegroup-core-full-cmdline \
        packagegroup-base-extended \
	openssh-sftp-server \
"

# Add to system packages
IMAGE_INSTALL_SYSTEM += "${IMAGE_INSTALL_MLUI_DEV}"

# Software packages for development image
IMAGE_INSTALL_MLUI_DEV = "\
	i2c-tools \
	usbutils \
	evtest \
"

# Image Features
IMAGE_FEATURES += "ssh-server-openssh dev-pkgs tools-sdk"

# Image Post-processing

