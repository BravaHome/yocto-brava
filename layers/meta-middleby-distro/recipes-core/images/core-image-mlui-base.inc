SUMMARY = "Base Image for MLUI Systems"
DESCRIPTION = "Provides the base image configuration. It is meant to be included by the image recipes."

LICENSE = "MIT"

inherit core-image

# Root File System Image Assembly
IMAGE_INSTALL = "\
  ${IMAGE_INSTALL_CORE} \
  ${IMAGE_INSTALL_BSP} \
  ${IMAGE_INSTALL_SYSTEM} \
  ${IMAGE_INSTALL_MEDIA} \
"

# Core Linux System Packages
IMAGE_INSTALL_CORE = "\
	packagegroup-core-boot \
	packagegroup-base \
	${CORE_IMAGE_EXTRA_INSTALL} \
"

# Board Support Packages
IMAGE_INSTALL_BSP = "\
	kernel-modules \
	mesa \
	wireless-regdb-static \
	linux-firmware-bcm43455 \
        u-boot-env \
"

# System Software Packages
IMAGE_INSTALL_SYSTEM = "\
        openssl \
        time \
        ntp \
        ntp-utils \
        ntp-tickadj \
	tzdata \
        fontconfig fontconfig-utils \
        liberation-fonts \
        alsa-utils \
	libmodbus \
	libgpiod \
	libgpiod-tools \
        networkmanager \
        networkmanager-nmcli \
        networkmanager-bash-completion \
	audio \
	udev-extraconf \
	lmsensors \
	libcap-bin \
	mlui-bluetooth \
	bluez5-noinst-tools \
        dialog \
        fbida \
        libubootenv \
        lsof \
        fbgrab \
        ${@bb.utils.contains('DISTRO_FEATURES', 'rauc', 'rauc', '', d)} \
"

# Media Software Packages
IMAGE_INSTALL_MEDIA = "\
        gstreamer1.0 \
        gst-examples \
        gstreamer1.0-plugins-base \
        gstreamer1.0-plugins-good \
        gstreamer1.0-plugins-bad \
        gstreamer1.0-libav \
"

# Image Features
IMAGE_FEATURES += "package-management"

# Image Post-processing
require include/user-setup.inc
require include/auto-login.inc
require include/system-version.inc
