# Image post-processing to auto-login user

# Auto-login user
AUTO_LOGIN_USER ??= ""

SERVICE_DIR = "${IMAGE_ROOTFS}/${sysconfdir}/systemd/system/getty@tty1.service.d"
HOME_DIR = "${IMAGE_ROOTFS}/home/${AUTO_LOGIN_USER}"

configure_auto_login() {
   # Setup automatic login
   if ! [ -z ${AUTO_LOGIN_USER} ]; then

      # Set getty @ tty1 for auto login
      if ${@bb.utils.contains('DISTRO_FEATURES','systemd','true','false',d)}; then
         install -m 0755 -d ${SERVICE_DIR}
	 printf "[Service]\nExecStart=\nExecStart=-/sbin/agetty -a ${AUTO_LOGIN_USER} -n -i %%I \$TERM\nType=idle" > ${SERVICE_DIR}/override.conf
      fi

      # Install files
      if [ -f ${THISDIR}/files/profile.${AUTO_LOGIN_USER} ]; then
         cat ${THISDIR}/files/profile.${AUTO_LOGIN_USER} >> ${HOME_DIR}/.profile
      fi
   fi

}
ROOTFS_POSTPROCESS_COMMAND:append = " configure_auto_login;"
