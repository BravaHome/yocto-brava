![ibeeto](artwork/ibeeto-logo.png "ibeeto")



# MLUI Linux Operating System Stack for Viking

Release notes for the Linux operating system stack for Viking.

## Image File Name Nomenclature

Image file names follow this nomenclature:

core-image-<company>-<use>-<machine>-<timestamp>.rootfs.wic[.gz]

Where:

* `<company>`:
  * viking - Image for Viking
  * synesso - Image for Synesso
* `<use>`:
  * prd - Image for production
  * dev - Image for development
* `<machine>`:
  * mlui-lcd7-usmp-t070 - MLUI with 7” US Micro Products display
  * mlui-lcd5 - MLUI with 5” Orient Display displayed
* `<timestamp>`:	Date and time in UTC when the image was built
* .gz: If present, indicates gzip compressed image that must be uncompressed before flashing

Compressed images, as indicated by the term *.gz* at the end of the image file name, are compressed using the *GNU Zip* compression utility. Linux systems provide the *gunzip* utility to uncompress the image:

```
gunzip core-image-<company>-<use>-<machine>-<timestamp>.rootfs.wic.gz
```

uncompresses the image *in place*, which means, that the uncompressed file replaces the compressed file.

```
gunzip -c  core-image-<company>-<use>-<machine>-<timestamp>.rootfs.wic.gz > core-image-synesso-prd-mlui-lcd5-20221221011449.rootfs.wic
```

uncompresses the image to a new file and retains the compressed file.

On Windows systems the command line utiltiy [GZip for Windows](https://gnuwin32.sourceforge.net/packages/gzip.htm) can be used. If a UI utility is preferred, [7-Zip](https://www.7-zip.org/) is a good choice.

To verify images after downlaod, copying, flashing etc. md5 checksums are provided of the uncompressed and compressed images. The md5 checksum files have the file name of the respective image file with the *.md5* added.

To check an image on Linux use:

```md5sum -c core-image-<company>-<use>-<machine>-<timestamp>.rootfs.wic[.gz].md5```

If the file checks correctly the output is:

```core-image-<company>-<use>-<machine>-<timestamp>.rootfs.wic[.gz]: OK```

To check on image on Windows use:

```Get-FileHash core-image-<company>-<use>-<machine>-<timestamp>.rootfs.wic[.gz] -Algorithm MD5```

The output shows the hash of the file but you have to compare it manually to the value in the file `core-image-<company>-<use>-<machine>-<timestamp>.rootfs.wic[.gz].md5`.

If you are flashing an eMMC or a SD card it is recommended that you read out the medium again, create the checksum and compare it to the provided checksum.


## Release 2023-06-06

### Wayland/Weston Backend

Wayland/Weston has been added as a backend for the graphical user interface. It is the default backend. To change it back to *eglfs* the file `/lib/systemd/system/virtuoso-ui.service` needs to be modified:

1. Comment out the lines (add a *#* at the beginning of each line):

   ```
   Environment="QT_QPA_PLATFORM=wayland"
   Environment="XDG_RUNTIME_DIR=/run"
   ```

2. Uncomment the line (remove the *#* from the beginning of the line):

   ```
   #Environment="QT_QPA_PLATFORM=eglfs"
   ```

Save the file and reboot the system.

### RAUC Software Updates

This release adds support for the [RAUC software update system](https://rauc.readthedocs.io/en/latest/). Please refer to the RAUC documentation and the README for details.


## Release 2023-03-06

Updated stepping of backlight intensity to account that first level from off is bright enough to read the screen.

Disabled auto-start of Freco applications as requested by Viking.


## Release 2023-01-13

### Partition Sizes
The size of the two root partitions A and B have been changed to 3GB each. The size of the data partition size has been reduced to 1.5GB. The remainder of the eMMC is occupied by boot loader and kernel.

### Virtuoso Application
The application for the Virtuoso oven has been updated to the latest version compiled with MLUI SDK. That now allows for proper packaging.


## Release 2022-12-20

### Development and Production Images
This release adds the production images to the previously provided development images. See section *Image File Name Nomeclature* above.

Production and development images differ as follows:

* Production images do not include development tools, headers and static libraries.
* Production images disable logins from serial and virtual terminals.
* Production images automatically run the MLUI test application on boot until all tests have completed successfully (see section *MLUI Test Application* below).
* Production images require keys for SSH login. The keys are preprogrammed for the *root* user.

### MLUI Test Application
To provide simple hardware testing after MLUI assembly the production images include a test application that launches automatically on boot. As long as the tests performed by the application have not completed successfully, the test application will always launch on boot. After all tests have completed successfully the test application reconfigures the system and the production application is then launched after boot.

### New Functionality
The following software packages have been added:

* libubootenv - Tools for writing the bootloader environment from Linux
* fbgrab - Tool to create screenshots from the framebuffer device

### Display Pixel Clock for US Micro Display
The pixel clock for the US Micro display has been increased to 60.4MHz to increase frame rate from approximately 20fps to 54fps.


## Release 2022-12-12

### Automount for FAT/VFAT/MSDOS File Systems
These file systems do not support user and group ownerships hence files and directories receive their
ownerships from the user who mounts them. Since automounting is performed by `udev` and `systemd` these
file system are mounted as the *root* user. This can result on isses when accessing directories and files
on removable drives, such as USB drives. 

This update resolves the issue by mounting the drive with group *disk* and setting the directory and file
permissions to read/write for the mount point `/media/<mountpoint>` and all directories and files being created.

### Regular Expressions for *systemd*
Filtering messages with `journalctl -g <filter>` requires *systemd* to be compiled with support for regular
expressions. This is by default not the case to make *systemd* more compact. This update adds support for
regular expressions to *systemd* (to be available for `journalctl`).


## Release 2022-10-11

### Viking Application
Latest version of Viking Application has been integrated.

### Fresco Kitchen Agent
The Fresco Kitchen Agent has been integrated.

### GPIO Hardware Revision Pins
The hardware revision pins are three input pins that are biased to indicate the MLUI hardware revision:
    
    PG10 202 HWREV0
    PG11 203 HWREV2
    PG12 204 HWREV1
    
To correctly read the hardware revision, the pins must be read in the following order: PG11, PG12, PG10.
    
For example, with gpiod:

```
root@mlui-usmp-t070:~# gpioget 1 203 204 202
0 1 1
```

The output `0 1 1` indicates a revision 3 MLUI.

 
### Murata WL Tool
The Murata WL tool is added to the development image. This tool allows testing of the Murata WiFi/BT chipset (as used by the Embedded Artists module) to be tested on the hardware level.

### USB Drive Automounting
The Yocto Project default USB drive automounting scheme crashes systemd if the device is unplugged when a
process changed directory into the mount point when the drive is unplugged.
    
The new scheme addresses this issue.


## Release 2022-08-17

### Bluetooth
Bluez5 has been updated to version 5.65

### GPIO
GPIO hogs are placed on SSR_IO output 1 and output 3 to turn off the MOSFET on startup. That prevents the corresponding input to be pulled to low which caused problem with an external interface board.

Since the GPIO driver now *hogs* the two GPIOs they cannot be controlled through user space anymore:

```
root@mlui-usmp-t070:~# gpioinfo 2
gpiochip2 - 8 lines:
        line   0:    "br_in_1"       unused   input  active-high 
        line   1:   "br_out_1"       unused   input  active-high 
        line   2:   "ssr_in_1"       unused   input  active-high 
        line   3:   "ssr_in_2"       unused   input  active-high 
        line   4:   "ssr_in_3"       unused   input  active-high 
        line   5:  "ssr_out_1" "ssr-out-1-hog" output active-high [used]
        line   6:  "ssr_out_2"       unused   input  active-high 
        line   7:  "ssr_out_3" "ssr-out-3-hog" output active-high [used]
```


## Release 2022-07-06

### Bluetooth
Bluetooth through the Embedded Artists 1MW WiFi/Bluetooth module is enabled on startup.

The module attaches to the BlueZ Bluetooth stack via `hci0`:

```
root@mlui-lcd7-usmp-t070:~# hciconfig
hci0:   Type: Primary  Bus: UART
        BD Address: AA:AA:AA:AA:AA:AA  ACL MTU: 1021:8  SCO MTU: 64:1
        UP RUNNING 
        RX bytes:746 acl:0 sco:0 events:45 errors:0
        TX bytes:1754 acl:0 sco:0 commands:45 errors:0
```

The utility `bluetoothctl` can be used to control BlueZ from the command line. The example scans for nearby Bluetooth devices, discoveres a *Lenovo YOGA Mouse*, and connects to it:

```
root@mlui-lcd7-usmp-t070:~# bluetoothctl 
Agent registered
[CHG] Controller AA:AA:AA:AA:AA:AA Pairable: yes
[bluetooth]# pairable on
Changing pairable on succeeded
[bluetooth]# scan on
Discovery started
[CHG] Controller AA:AA:AA:AA:AA:AA Discovering: yes
[NEW] Device F2:6A:3E:3D:5B:EA Lenovo YOGA Mouse
[CHG] Device F2:6A:3E:3D:5B:EA RSSI: -64
[bluetooth]# connect F2:6A:3E:3D:5B:EA
Attempting to connect to F2:6A:3E:3D:5B:EA
[CHG] Device F2:6A:3E:3D:5B:EA Connected: yes
Connection successful
[Lenovo YOGA Mouse]#
[NEW] Primary Service (Handle 0x8920)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service0008
        00001801-0000-1000-8000-00805f9b34fb
        Generic Attribute Profile
[NEW] Characteristic (Handle 0x0a38)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service0008/char0009
        00002a05-0000-1000-8000-00805f9b34fb
        Service Changed
[NEW] Descriptor (Handle 0xbeb4)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service0008/char0009/desc000b
        00002902-0000-1000-8000-00805f9b34fb
        Client Characteristic Configuration
[NEW] Primary Service (Handle 0x2e30)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service000c
        0000180a-0000-1000-8000-00805f9b34fb
        Device Information
[NEW] Characteristic (Handle 0x0a38)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service000c/char000d
        00002a29-0000-1000-8000-00805f9b34fb
        Manufacturer Name String
[NEW] Characteristic (Handle 0x0a38)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service000c/char000f
        00002a26-0000-1000-8000-00805f9b34fb
        Firmware Revision String
[NEW] Characteristic (Handle 0x0a38)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service000c/char0011
        00002a50-0000-1000-8000-00805f9b34fb
        PnP ID
[NEW] Primary Service (Handle 0x68c0)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service0013
        0000180f-0000-1000-8000-00805f9b34fb
        Battery Service
[NEW] Characteristic (Handle 0x0a38)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service0013/char0014
        00002a19-0000-1000-8000-00805f9b34fb
        Battery Level
[NEW] Descriptor (Handle 0xbeb4)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service0013/char0014/desc0016
        00002902-0000-1000-8000-00805f9b34fb
        Client Characteristic Configuration
[NEW] Primary Service (Handle 0x94b0)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service0017
        00001530-1212-efde-1523-785feabcd123
        Vendor specific
[NEW] Characteristic (Handle 0x0a38)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service0017/char0018
        00001532-1212-efde-1523-785feabcd123
        Vendor specific
[NEW] Characteristic (Handle 0x0a38)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service0017/char001a
        00001531-1212-efde-1523-785feabcd123
        Vendor specific
[NEW] Descriptor (Handle 0xbeb4)
        /org/bluez/hci0/dev_F2_6A_3E_3D_5B_EA/service0017/char001a/desc001c
        00002902-0000-1000-8000-00805f9b34fb
        Client Characteristic Configuration
[CHG] Device F2:6A:3E:3D:5B:EA UUIDs: 00001530-1212-efde-1523-785feabcd123
[CHG] Device F2:6A:3E:3D:5B:EA UUIDs: 00001800-0000-1000-8000-00805f9b34fb
[CHG] Device F2:6A:3E:3D:5B:EA UUIDs: 00001801-0000-1000-8000-00805f9b34fb
[CHG] Device F2:6A:3E:3D:5B:EA UUIDs: 0000180a-0000-1000-8000-00805f9b34fb
[CHG] Device F2:6A:3E:3D:5B:EA UUIDs: 0000180f-0000-1000-8000-00805f9b34fb
[CHG] Device F2:6A:3E:3D:5B:EA UUIDs: 00001812-0000-1000-8000-00805f9b34fb
[CHG] Device F2:6A:3E:3D:5B:EA ServicesResolved: yes
[CHG] Device F2:6A:3E:3D:5B:EA WakeAllowed: yes
[CHG] Device F2:6A:3E:3D:5B:EA Modalias: usb:v17EFp608Ad0001
[Lenovo YOGA Mouse]# info F2:6A:3E:3D:5B:EA
Device F2:6A:3E:3D:5B:EA (random)
        Name: Lenovo YOGA Mouse
        Alias: Lenovo YOGA Mouse
        Appearance: 0x03c2
        Icon: input-mouse
        Paired: no
        Trusted: no
        Blocked: no
        Connected: yes
        WakeAllowed: yes
        LegacyPairing: no
        UUID: Vendor specific           (00001530-1212-efde-1523-785feabcd123)
        UUID: Generic Access Profile    (00001800-0000-1000-8000-00805f9b34fb)
        UUID: Generic Attribute Profile (00001801-0000-1000-8000-00805f9b34fb)
        UUID: Device Information        (0000180a-0000-1000-8000-00805f9b34fb)
        UUID: Battery Service           (0000180f-0000-1000-8000-00805f9b34fb)
        UUID: Human Interface Device    (00001812-0000-1000-8000-00805f9b34fb)
        Modalias: usb:v17EFp608Ad0001
        RSSI: -64
        Battery Percentage: 0x64 (100)
[bluetooth]# trust F2:6A:3E:3D:5B:EA
[CHG] Device F2:6A:3E:3D:5B:EA Trusted: yes
Changing F2:6A:3E:3D:5B:EA trust succeeded
```