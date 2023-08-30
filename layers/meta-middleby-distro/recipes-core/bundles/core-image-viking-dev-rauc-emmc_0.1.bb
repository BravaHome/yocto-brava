SUMMARY = "RAUC Bundle for Viking MLUI on eMMC"
DESCRITPION = "This buundle updates the eMMC"

require viking.inc

RAUC_BUNDLE_DESCRIPTION = "RAUC Bundle for Viking MLUI"

RAUC_BUNDLE_SLOTS = "emmc-kernel emmc-rootfs" 
RAUC_SLOT_emmc-kernel = "core-image-kernel"
RAUC_SLOT_emmc-kernel[fstype] = "ext4"
RAUC_SLOT_emmc-rootfs = "core-image-viking-dev"
RAUC_SLOT_emmc-rootfs[fstype] = "ext4"
