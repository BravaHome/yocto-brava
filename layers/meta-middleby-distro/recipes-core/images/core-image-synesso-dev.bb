SUMMARY = "Development Image for Synesso systems based on MLUI"
DESCRIPTION = "Image for the Synesso configuration building on top of the base image."

LICENSE = "MIT"

require core-image-synesso-base.inc

# Add to system packages
IMAGE_INSTALL_SYSTEM += "${IMAGE_INSTALL_SYNESSO_DEV}"

# Software Packages for the development image
IMAGE_INSTALL_SYNESSO_DEV = "\
	i2c-tools \
	evtest \
"

EXTRA_IMAGE_FEATURES = "debug-tweaks ssh-server-openssh dev-pkgs tools-sdk tools-debug tools-profile"

