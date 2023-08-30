#!/bin/bash
# (c) 2022, ibeeto, Rudolf J Streif
# Connect to MLUI system via ssh/sftp

# Code Section

SCRIPT_PATH="$( cd "$(dirname ${0})" >/dev/null 2>&1 ; pwd -P )"
PROJECT_PATH="$(dirname ${SCRIPT_PATH})"
BUILD_PATH="${PROJECT_PATH}/build"
DEPLOY_PATH="${PROJECT_PATH}/build/deploy"
KEY_PATH="${PROJECT_PATH}/build/deploy/keys"
USER=$1
HOST=$2

[ -z "${USER}" ] && echo "User name not specified" && exit 1

/usr/bin/ssh -o IdentitiesOnly=yes -o StrictHostKeyChecking=no -i ${KEY_PATH}/${USER}-sshkey ${USER}@${HOST}

