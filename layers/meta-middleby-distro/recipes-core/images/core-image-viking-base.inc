SUMMARY = "Base image for Brava/Viking systems based on MLUI"
DESCRIPTION = "Image for the Brava/Viking configuration building on top of the MLUI base image. \
               This file is meant to be included by the actual image recipe. \
              "

LICENSE = "MIT"

require core-image-mlui-base.inc

# Add to system packages
IMAGE_INSTALL_SYSTEM += "${IMAGE_INSTALL_GRAPHICS} ${IMAGE_INSTALL_QT} ${IMAGE_INSTALL_VIKING} ${IMAGE_INSTALL_PYTHON}"

# Graphics Packages
IMAGE_INSTALL_GRAPHICS = "\
	weston \
	weston-init \
	weston-examples \
	"

# Software Packages for the base image
IMAGE_INSTALL_VIKING = "\
    alsa-tools \
    mpv \
    openjpeg \
    v4l-utils \
    vorbis-tools \
    bzip2 \
    gzip \
    unzip \
    zip \
    dool \
    binutils \
    diffutils \
    findutils \
    gawk \
    grep \
    sed \
    tree \
    dosfstools \
    e2fsprogs \
    ca-certificates \
    gnutls \
    libssh2 \
    mbedtls \
    db \
    sqlite \
    libconfig \
    libdrm-tests \
    glew \
    gnu-efi \
    libmbim \
    libcurl \
    elfutils \
    msgpack-c \
    libnewt \
    libpcre2 \
    dhcpcd \
    iptables \
    iw \
    netcat \
    netcat-openbsd \
    networkmanager \
    networkmanager-nmcli \
    networkmanager-bash-completion \
    wget \
    wpa-supplicant \
    file \
    coreutils \
    imagemagick \
    mesa \
    lmsensors \
    nodejs \
    tar \
    util-linux \
    joe \
    nano \
    vim \
    libdbus-c++ \
    mosquitto \
    egalax-i2c \
    virtuoso \
    r485c \
    kos-agent \
    kos-agent-systemd \
    libatomic \
"

# Packages requiring X11
IMAGE_INSTALL_X11 = "\
    ${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'libglu', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'startup-notification', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'vlc', '', d)} \
"

# Python Software Packages
IMAGE_INSTALL_PYTHON = "\
    python3 \
    python3-setuptools \
    python3-pyserial \
    python3-cffi \
    python3-markdown \
    python3-pyyaml \
    python3-pyzmq \
    pymodbus \
"

# Qt Packages
IMAGE_INSTALL_QT = "\
        qtbase \
        qtbase-plugins \
        qtbase-qmlplugins \
        qtbase-tools \
        qtconnectivity \
        qtconnectivity-qmlplugins \
        qtdeclarative \
        qtdeclarative-dev \
        qtdeclarative-tools \
        qtdeclarative-qmlplugins \
        qtgraphicaleffects \
        qtgraphicaleffects-qmlplugins \
        qtquickcontrols \
        qtquickcontrols-qmlplugins \
        qtquickcontrols2 \
        qtquickcontrols2-qmlplugins \
        qtimageformats \
        qtsvg \
        qtmultimedia \
        qtmultimedia-qmlplugins \
        qtserialport \
        qtwebkit \
        qtwayland \
"

require include/getty-setup.inc

IMAGE_OVERHEAD_FACTOR="1.1"
