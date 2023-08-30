FILESEXTRAPATHS:prepend := "${THISDIR}/udev-extraconf:"

SRC_URI += "file://localextra.rules \
            file://${POST_CONFIGURATION}.rules \
            file://media-automount.sh \
            file://media-automount.rules \
            file://media-automount@.service \
            file://vfat \
           "

POST_CONFIGURATION ??= ""

do_install:append() {
    install -Dm0644 ${WORKDIR}/localextra.rules ${D}${sysconfdir}/udev/rules.d/localextra.rules

    install -Dm0644 ${WORKDIR}/${POST_CONFIGURATION}.rules ${D}${sysconfdir}/udev/rules.d/${POST_CONFIGURATION}.rules

    # USB drive automounting
    install -Dm0644 ${WORKDIR}/media-automount.rules ${D}${sysconfdir}/udev/rules.d/media-automount.rules
    install -Dm0755 ${WORKDIR}/media-automount.sh ${D}${sysconfdir}/udev/scripts/media-automount.sh
    install -Dm0644 ${WORKDIR}/media-automount@.service ${D}${sysconfdir}/systemd/system/media-automount@.service
    install -Dm0644 ${WORKDIR}/vfat ${D}${sysconfdir}/media-automount.d/vfat

    # remove default automounting
    rm ${D}${sysconfdir}/udev/rules.d/automount.rules
    rm ${D}${sysconfdir}/udev/scripts/mount.sh
}

FILES:${PN} += "${sysconfdir}/systemd ${sysconfdir}/media-automount.d"
