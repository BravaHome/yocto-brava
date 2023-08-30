SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "kOS Agent"
LICENSE = "CLOSED"

SRC_URI = "\
       file://kos-agent \
       file://dbus-policy.conf \
       file://start-provision.sh \
       file://stop-provision.sh \
       file://disconnect.sh"

S = "${WORKDIR}"

do_compile[noexec] = "1"

do_install() {
       install -d ${D}${bindir}
       install -m 0755 ${S}/kos-agent ${D}${bindir}

       install -d ${D}${sysconfdir}/dbus-1/system.d
       install -m 0644 ${S}/dbus-policy.conf ${D}${sysconfdir}/dbus-1/system.d/kos-agent.conf

       install -d ${D}${ROOT_HOME}
       install -m 0755 ${S}/*.sh ${D}${ROOT_HOME}
}

RDEPENDS:${PN}-dev += "bash"

FILES:${PN}-dev = "\
       ${ROOT_HOME}/*.sh"

# Avoid "kos-agent in package agent doesn't have GNU_HASH (didn't pass LDFLAGS?)" error
# due to compiling the binary manually instead of doing it in a recipe with `inherit go`
INSANE_SKIP:${PN} = "ldflags"
INSANE_SKIP:${PN}-dev = "ldflags"
