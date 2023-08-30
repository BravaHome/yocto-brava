FILESEXTRAPATHS:prepend := "${THISDIR}/linux-firmware:"

SRC_URI += "file://brcmfmac43455-sdio.middleby,mlui.txt \
"


do_install:append() {
   install -m 0644 ${WORKDIR}/brcmfmac43455-sdio.middleby,mlui.txt ${D}${nonarch_base_libdir}/firmware/brcm
   ln -sf -r ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43455-sdio.middleby,mlui.txt ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43455-sdio.middleby,mlui-lcd5.txt
   ln -sf -r ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43455-sdio.middleby,mlui.txt ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43455-sdio.middleby,mlui-lcd7.txt
   ln -sf -r ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43455-sdio.middleby,mlui.txt ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43455-sdio.middleby,mlui-lcd7-usmp-t070.txt
}

