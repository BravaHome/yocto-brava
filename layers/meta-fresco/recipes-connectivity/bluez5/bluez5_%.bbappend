# bluez5 enables a "battery" plugin by default that causes issues with iOS, so we need to disable it:
do_install:append() {
    sed -i 's:^\(ExecStart=/usr/libexec/bluetooth/bluetoothd\)$:\1 -P battery:' \
        ${D}/lib/systemd/system/bluetooth.service;
}
