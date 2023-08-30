LICENSE = "CLOSED"
inherit systemd

SRC_URI = "\
	file://kos-agent.service \
	file://kos-agent-dev.service"

SYSTEMD_PACKAGES = "${PN} ${PN}-dev"

SYSTEMD_SERVICE:${PN} = "kos-agent.service"
FILES:${PN} += "${systemd_unitdir}/system/kos-agent.service"

SYSTEMD_SERVICE:${PN}-dev = "kos-agent-dev.service"
FILES:${PN}-dev += "${systemd_unitdir}/system/kos-agent-dev.service"

SYSTEMD_AUTO_ENABLE = "disable"

do_install:append() {
  install -d ${D}/${systemd_unitdir}/system
  install -m 0644 ${WORKDIR}/kos-agent.service ${D}/${systemd_unitdir}/system
  install -m 0644 ${WORKDIR}/kos-agent-dev.service ${D}/${systemd_unitdir}/system
}
