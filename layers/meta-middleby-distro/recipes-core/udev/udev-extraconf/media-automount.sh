#!/bin/sh
#$1 = <dev>

# Default options to use for mounting
AUTOMOUNT_OPTS='errors=remount-ro,relatime,user'
# Default type to use for mounting
AUTOMOUNT_TYPE='auto'

# Directory to look for type-specific settings
confdir=/etc/media-automount.d

# Directory to use as parent media dir for mountpoints
mediadir=/media

[[ $EUID != 0 ]] && {
    echo "This tool requires root permissions"
    exit 1
}
shopt -s nullglob

log() {
    logger -st "media-automount" "$*"
}

if ! [ "$1" ]
then
    log "missing arguments! a device name must be provided"
    exit 1
else
    dev=/dev/${1##*/}  
fi

# Check if the device exists, if not but mounted, umount it
if ! [ -b $dev ]
then
    if grep /etc/mtab -qe "^$dev"
    then
        log "$dev device removed, umounting and cleaning /media"
        if umount -l "$dev"
        then
            exitcode=0
        else
            exitcode=$?
            log "Error umounting $dev errcode:$exitcode"
            log "Command was: umount $dev"
        fi
    else
        # prevent it from failing on nonexistent devices and degrading systemctl boot
        exitcode=0
		log "device doesn't exist anymore or is not a block device: $dev"
    fi

    # cleanup
    for dir in "$mediadir"/*
    do
        [ -d "$dir" ] && ! mountpoint -q "$dir" && rmdir "$dir"
    done
    exit $exitcode
fi

# Load additional info for the block device
eval $(blkid -po export $dev)

# Devices with unknown type will be ignored
if [ -z "$TYPE" ]
then
    log "$dev has no known filesystem type, ignoring mount request"
	exit 0
fi

# Check /etc/fstab for an entry corresponding to the device
[ "$UUID" ] && fstab=$(grep /etc/fstab -e "^[^#]*${UUID/\-/\\\-}") || \
[ "$LABEL" ] && fstab=$(grep /etc/fstab -e "^[^#]*${LABEL/\-/\\\-}") || \
fstab=$(grep /etc/fstab -e "^[ \t]*$dev[ \t]")

# Don't manage devices that are already in fstab
if [ "$fstab" ]
then
    log "$dev already in /etc/fstab, automount won't manage it: ${fstab/[ \t][ \t]/ }"
    exit 1
fi

# directory name
AUTOMOUNT_DIR="${mediadir}/${LABEL:-${dev##*/}}"

# Load Filesystem-specific configuration for mounting
if [ -e "$confdir/$TYPE" ]
then
        log "loading configuration for fs type $TYPE"
	source "$confdir/$TYPE"
elif [ -e "$confdir/auto" ]
then
	source "$confdir/auto"
fi

log "mounting device $dev in $AUTOMOUNT_DIR"
mkdir -p "$AUTOMOUNT_DIR"
if mount -t "$AUTOMOUNT_TYPE" -o "$AUTOMOUNT_OPTS" "$dev" "$AUTOMOUNT_DIR"
then
    # Notify
    username="$(ps au | awk '$11 ~ /^xinit/ { print $1; exit }')"
    [[ "$username" ]] && DISPLAY=:0 runuser -u "$username" xdg-open "$AUTOMOUNT_DIR"
    log "Device successfully mounted: $AUTOMOUNT_DIR"
    exit 0
else
    log "Mount error: $?"
    log "Command was : mount -t $AUTOMOUNT_TYPE -o $AUTOMOUNT_OPTS $dev $AUTOMOUNT_DIR"

    rmdir "$AUTOMOUNT_DIR"
    exit 1
fi