SUMMARY = "Image for Synesso systems based on MLUI"
DESCRIPTION = "Image for the Synesso configuration building on top of the base image."

LICENSE = "MIT"

require core-image-synesso-base.inc

# Add to system packages
IMAGE_INSTALL_SYSTEM += "${IMAGE_INSTALL_SYNESSO_PRD}"

# Software Packages for the production image
IMAGE_INSTALL_SYNESSO_PRD = "\
    mlui-test \
"

EXTRA_IMAGE_FEATURES = "ssh-server-openssh"

require include/sshd-setup.inc
