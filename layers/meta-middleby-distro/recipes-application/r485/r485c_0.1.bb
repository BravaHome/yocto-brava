SUMMARY = "Hello with Makefile"
DESCRIPTION = "A test application to demonstrate how to create a \
               recipe for makefile-based project."

SECTION = "examples"
PRIORITY = "optional"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI =  "file://Makefile \
            file://r485.h \
            file://r485c.c \
            file://r485lib.h \
            file://r485lib.c \
"

S = "${WORKDIR}"

EXTRA_OEMAKE = "'CC=${CC}' 'RANLIB=${RANLIB}' 'AR=${AR}' \
   'CFLAGS=${CFLAGS} -I${S}/. -DWITHOUT_XATTR' 'BUILDDIR=${S}'"

do_install () {
   oe_runmake install DESTDIR=${D} BINDIR=${bindir} SBINDIR=${sbindir} \
      MANDIR=${mandir} INCLUDEDIR=${includedir}
}

INSANE_SKIP:${PN} = "ldflags"

