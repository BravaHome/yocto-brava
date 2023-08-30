FILESEXTRAPATHS:prepend := "${THISDIR}/systemd:"

SRC_URI:append = " file://touchscreen.rules \
                 "

PACKAGECONFIG:append = " pcre2"
