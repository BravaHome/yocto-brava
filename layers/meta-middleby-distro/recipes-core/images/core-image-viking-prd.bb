SUMMARY = "Image for Brava/Viking systems based on MLUI"
DESCRIPTION = "Image for the Brava/Viking configuration building on top of the base image."

LICENSE = "MIT"

require core-image-viking-base.inc

# Add to system packages
IMAGE_INSTALL_SYSTEM += "${IMAGE_INSTALL_VIKING_PRD}"

# Software Packages for the production image
IMAGE_INSTALL_VIKING_PRD = "\
    mlui-test \
"

EXTRA_IMAGE_FEATURES = "ssh-server-openssh"

require include/sshd-setup.inc
