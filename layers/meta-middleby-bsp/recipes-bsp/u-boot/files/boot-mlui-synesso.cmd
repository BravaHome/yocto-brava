# Synesso u-boot boot script for MLUI

# Kernel partition
kernelpart=2

# Root file system partition
rootpart=3

# ${devnum} is set by the u-boot environment
rootdev=mmcblk${devnum}p${rootpart}

if test ${devnum} -eq 1; then
	# U-Boot loaded from eMMC
	# Linux enumerates the eMMC device as 2 while u-boot uses 1 on the MLUI
	echo "U-boot loaded from eMMC or secondary SD"
	rootdev=mmcblk2p${rootpart}
fi

setenv bootargs console=${console} vt.global_cursor_default=0 root=/dev/${rootdev} rootwait panic=10 ${extra}
load mmc ${devnum}:${kernelpart} ${fdt_addr_r} ${fdtfile}
load mmc ${devnum}:${kernelpart} ${kernel_addr_r} Image
booti ${kernel_addr_r} - ${fdt_addr_r}
