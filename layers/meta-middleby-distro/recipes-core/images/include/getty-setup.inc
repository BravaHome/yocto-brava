# Image post-processing to disable tty1

GETTY_TTY1 = "${IMAGE_ROOTFS}/${sysconfdir}/systemd/system/getty@tty1.service"

setup_tty() {
   if [ ! -f ${GETTY_TTY1} ]; then
      ln -s /dev/null ${GETTY_TTY1}
   fi
}
ROOTFS_POSTPROCESS_COMMAND:append = " setup_tty;"
