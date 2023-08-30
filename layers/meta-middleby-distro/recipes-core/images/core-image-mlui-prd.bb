SUMMARY = "Image for MLUI board"
DESCRIPTION = "Image for the MLUI default configuration building on top of the base image."

LICENSE = "MIT"

require core-image-mlui-base.inc

# Add to system packages
IMAGE_INSTALL_SYSTEM += "${IMAGE_INSTALL_MLUI_PRD}"

# Software Packages for the production image
IMAGE_INSTALL_MLUI_PRD = "\
"
