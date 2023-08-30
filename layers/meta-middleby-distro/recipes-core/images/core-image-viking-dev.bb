SUMMARY = "Development Image for Brava/Viking systems based on MLUI"
DESCRIPTION = "Image for the Brava/Viking configuration building on top of the base image."

LICENSE = "MIT"

require core-image-viking-base.inc

# Add to system packages
IMAGE_INSTALL_SYSTEM += "${IMAGE_INSTALL_VIKING_DEV}"

# Software Packages for the development image
IMAGE_INSTALL_VIKING_DEV = "\
    iozone3 \
    lsof \
    netperf \
    strace \
    tinymembench \
    libtool \
    iperf3 \
    rsync \
    screen \
    sudo \
    time \
    which \
    android-tools \
    debianutils \
    mmc-utils \
    fb-test \
    hdparm \
    i2c-tools \
    lshw \
    memtester \
    parted \
    picocom \
    htop \
    iotop \
    procps \
    systemd-bootchart \
    oprofile \
    stress-ng \
    evtest \
    openssh-sftp-server \
    wl \
    glmark2 \
"

EXTRA_IMAGE_FEATURES = "debug-tweaks ssh-server-openssh dev-pkgs tools-sdk tools-debug tools-profile"

