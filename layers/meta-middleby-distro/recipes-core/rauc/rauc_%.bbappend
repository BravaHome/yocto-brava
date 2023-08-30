FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://system-viking.conf \
            file://ca.cert.pem \
           "


do_configure:prepend() {
    cp ${WORKDIR}/system-viking.conf ${WORKDIR}/system.conf
}
