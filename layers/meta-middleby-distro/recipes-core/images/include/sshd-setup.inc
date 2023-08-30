# Image post-processing to configure sshd

# Setup ssh key login for these users
SSH_USERS ??= ""
SSH_DISALLOW_PWAUTH ??= "1"

configure_sshd() {
   # disallow password authentication
   if [ "${SSH_DISALLOW_PWAUTH}" == "1" ]; then
      echo "PasswordAuthentication no" >> ${IMAGE_ROOTFS}/etc/ssh/sshd_config
   fi

   # keys will be stored tmp/deploy/keys
   mkdir -p ${DEPLOY_DIR}/keys

   # create the keys for the users
   for user in ${SSH_USERS}; do
      if [ ! -f ${DEPLOY_DIR}/keys/${user}-sshkey ]; then
      /usr/bin/ssh-keygen -t rsa -N '' \
         -f ${DEPLOY_DIR}/keys/${user}-sshkey
      fi

      # add public key to authorized_keys for the user
      mkdir -p ${IMAGE_ROOTFS}/home/${user}/.ssh
      cat ${DEPLOY_DIR}/keys/${user}-sshkey.pub \
          >> ${IMAGE_ROOTFS}/home/${user}/.ssh/authorized_keys
   done
}
ROOTFS_POSTPROCESS_COMMAND += "configure_sshd;"

