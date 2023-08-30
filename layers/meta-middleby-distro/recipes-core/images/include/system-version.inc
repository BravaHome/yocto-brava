# Image post processing to add a system/build version file

# Version File
SYSTEM_VERSION_FILE ??= "system-version.txt"

SYSTEM_VERSION_DIR = "${IMAGE_ROOTFS}/${sysconfdir}"

add_system_version() {
   # Create system versin file
   if [ -n ${SYSTEM_VERSION_FILE} ]; then
      image_version="$(awk -F'-' '{print $2}' <<< ${IMAGE_VERSION_SUFFIX})"
      printf "[IMAGE]  ${IMAGE_NAME} ${image_version}\n" > ${SYSTEM_VERSION_DIR}/${SYSTEM_VERSION_FILE}
      printf "[BUILD]  ${BUILD_SYS} ${BUILD_OS}\n" >> ${SYSTEM_VERSION_DIR}/${SYSTEM_VERSION_FILE}
      printf "[TARGET] ${TARGET_SYS} ${TARGET_OS}\n" >> ${SYSTEM_VERSION_DIR}/${SYSTEM_VERSION_FILE}
   fi
}
ROOTFS_POSTPROCESS_COMMAND:append = " add_system_version;"
