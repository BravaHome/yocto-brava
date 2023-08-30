SUMMARY = "Synesso Single Brew Group Espresso System"
LICENSE = "CLOSED"

SRC_URI = "file://middleby_setup_scripts-2020-10-26.tgz \
           file://synesso-${MDB_RELEASE}.tgz \
           file://middleby_ui.env \
           file://mdb_ui.service \
          "

MDB_SCRIPT_DIR = "${D}/opt/middleby_setup_scripts"
MDB_BASELINE_DIR = "${D}/opt/middleby_baseline"
SYSCONF_SYSTEMD = "${D}/${sysconfdir}/systemd/system"

inherit useradd

do_install:append () {
    install -d -m 0755 ${MDB_SCRIPT_DIR}
    install -d -m 0755 ${MDB_BASELINE_DIR}
    install -d -m 0755 ${SYSCONF_SYSTEMD}
    install -d -m 0755 ${SYSCONF_SYSTEMD}/multi-user.target.wants
    
    tar cf - -C ${WORKDIR}/opt/middleby_setup_scripts . | tar xf - -C ${MDB_SCRIPT_DIR}
    tar cf - -C ${WORKDIR}/opt/middleby-${MDB_RELEASE} --exclude={"linux-x86","linux-x86_64","macos-x86","win32-x86","libopenal.so.1","libfbxsdk.so"} . | tar xf - -C ${MDB_BASELINE_DIR}

    install -m 0644 ${MDB_SCRIPT_DIR}/mdb_update.service ${SYSCONF_SYSTEMD}
    install -m 0644 ${MDB_SCRIPT_DIR}/mdb_usb.service ${SYSCONF_SYSTEMD}
    install -m 0644 ${MDB_SCRIPT_DIR}/mdb_watchdog.service ${SYSCONF_SYSTEMD}
    install -m 0644 ${MDB_SCRIPT_DIR}/mdb_wifi.service ${SYSCONF_SYSTEMD}
    install -m 0644 ${WORKDIR}/mdb_ui.service ${SYSCONF_SYSTEMD}
    install -m 0644 ${WORKDIR}/middleby_ui.env ${MDB_BASELINE_DIR}/scripts

    pushd ${D}/opt
    ln -s middleby_baseline ${D}/opt/middleby
    popd
    pushd ${SYSCONF_SYSTEMD}/multi-user.target.wants
    ln -s ../mdb_update.service ${SYSCONF_SYSTEMD}/multi-user.target.wants/mdb_update.service
    ln -s ../mdb_usb.service ${SYSCONF_SYSTEMD}/multi-user.target.wants/mdb_usb.service
    ln -s ../mdb_watchdog.service ${SYSCONF_SYSTEMD}/multi-user.target.wants/mdb_watchdog.service
    ln -s ../mdb_wifi.service ${SYSCONF_SYSTEMD}/multi-user.target.wants/mdb_wifi.service
    ln -s ../mdb_ui.service ${SYSCONF_SYSTEMD}/multi-user.target.wants/mdb_ui.service
    popd

    chown -R synesso:synesso ${MDB_SCRIPT_DIR}
    chown -R synesso:synesso ${MDB_BASELINE_DIR}
    chmod 0777 ${D}/opt
}

FILES:${PN} = "/opt/middleby /opt/middleby_baseline /opt/middleby_setup_scripts ${sysconfdir}"

USERADD_PACKAGES = "${PN}"
USERADD_PARAM:${PN} = "--home /home/synesso --shell /bin/sh --user-group -G dialout,systemd-journal,disk synesso"

RDEPENDS:${PN} += "libsystemd libudev libgpiod wayland weston libegl-mesa libgles2-mesa bash libmodbus"
INSANE_SKIP:${PN} += "ldflags arch staticdev"
