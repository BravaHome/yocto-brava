# Image post-processing to add virtuoso executable
# This is a kludge since the application was not built correctly

VIRTUOSO_DIR = "${IMAGE_ROOTFS}/opt/virtuoso"

copy_virtuoso() {
   # Install files
   if [ -f ${THISDIR}/files/virtuoso.a64 ]; then
      install -m 0775 ${THISDIR}/files/virtuoso.a64 ${VIRTUOSO_DIR}/virtuoso
   fi
}
ROOTFS_POSTPROCESS_COMMAND:append = " copy_virtuoso;"
