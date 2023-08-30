SUMMARY = "RAUC Bundle for Viking MLUI on SD Card"
DESCRITPION = "This buundle updates the SD card"

require viking.inc

RAUC_BUNDLE_DESCRIPTION = "RAUC Bundle for Viking MLUI"

RAUC_BUNDLE_SLOTS = "sd-kernel sd-rootfs" 
RAUC_SLOT_sd-kernel = "core-image-kernel"
RAUC_SLOT_sd-kernel[fstype] = "ext4"
RAUC_SLOT_sd-rootfs = "core-image-viking-prd"
RAUC_SLOT_sd-rootfs[fstype] = "ext4"

