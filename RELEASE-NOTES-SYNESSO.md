![ibeeto](artwork/ibeeto-logo.png "ibeeto")



# MLUI Linux Operating System Stack for Synesso

Release notes for the Linux operating system stack for Synesso.

## Image File Name Nomenclature

Image file names follow this nomenclature:

core-image-<company>-<use>-<machine>-<timestamp>.rootfs.wic[.gz]

Where:

* `<company`>`:
  * viking - Image for Viking
  * synesso - Image for Synesso
* `<use>`:
  * prd - Image for production
  * dev - Image for development
* `<machine>`:
  * mlui-lcd7-usmp-t070 - MLUI with 7” US Micro Products display
  * mlui-lcd5 - MLUI with 5” Orient Display displayed
* `<timestamp>`: Date and time in UTC when the image was built
* .gz: If present, indicates gzip compressed image that must be uncompressed before flashing

Compressed images, as indicated by the term *.gz* at the end of the image file name, are compressed using the *GNU Zip* compression utility. Linux systems provide the *gunzip* utility to uncompress the image:

`gunzip core-image-<company>-<use>-<machine>-<timestamp>.rootfs.wic.gz` uncompresses the image *in place*, which means, that the uncompressed file replaces the compressed file.

`gunzip -c  core-image-<company>-<use>-<machine>-<timestamp>.rootfs.wic.gz > core-image-synesso-prd-mlui-lcd5-20221221011449.rootfs.wic` uncompresses the image to a new file and retains the compressed file.

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


## Release 2023-03-06

Updated stepping of backlight intensity to account that first level from off is bright enough to read the screen.


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

### GPIO Hardware Revision Pins
The hardware revision pins are three input pins that are biased to indicate the MLUI hardware revision:
    
    PG10 202 HWREV0
    PG11 203 HWREV2
    PG12 204 HWREV1
    
To correctly read the hardware revision, the pins must be read in the following order: PG11, PG12, PG10.
    
For example, with gpiod:
    
```
root@mlui-lcd5:~# gpioget 1 203 204 202
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

## Release 2022-07-06

### Bluetooth
Bluetooth through the Embedded Artists 1MW WiFi/Bluetooth module is enabled on startup.

The module attaches to the BlueZ Bluetooth stack via `hci0`:

```
root@mlui-lcd5:~# hciconfig
hci0:   Type: Primary  Bus: UART
        BD Address: AA:AA:AA:AA:AA:AA  ACL MTU: 1021:8  SCO MTU: 64:1
        UP RUNNING 
        RX bytes:746 acl:0 sco:0 events:45 errors:0
        TX bytes:1754 acl:0 sco:0 commands:45 errors:0
```

The utility `bluetoothctl` can be used to control BlueZ from the command line. The example scans for nearby Bluetooth devices, discoveres a *Lenovo YOGA Mouse*, and connects to it:

```
root@mlui-lcd5:~# bluetoothctl 
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

## Release 2022-06-24

### Synesso Espresso Machine Application
Updated to version 2022-06-23 20:13.
Changed systemd service to set scheduling and IO priorities.

## Release 2022-06-09

### User `synesso`
The user synesso is set on the system to run the espresso maker application software. The home directory is `/home/synesso`. The user is added to the `dialout` user group to allow control of RS485 ports. The user is also allowed to control the gpio's provided by `/dev/gpiochip2` which is the I2C gpio expander controlling the I/O lines on the SSR_IO and BR_IO connectors.

### User Auto Login with Weston Start
The user `synesso` is automatically logged in startup and Weston is started.

### Capabilities Support
Kernel supports capabilities allowing for regular user-space processes to be granted capabilities normally reserved for `root`.

### Synesso Splash Image
The kernel shows the Synesso logo on startup until user-space is launched and Weston is started.

### Version File
A version file is added to the root file system: `/etc/system-version.txt`. It contains the following information:

```
[IMAGE]  core-image-synesso-dev-mlui-lcd5-20220609201844 20220609201844
[BUILD]  x86_64-linux linux
[TARGET] aarch64-middleby-linux linux
```

* `[IMAGE]` The name of the system image and the image timestamp in UTC.
* `[BUILD]` Build system information
* `[TARGET]` Target system information

The buils system automatically updates the version information on each image build.

### Synesso Espresso Machine Application
The Synesso espresso machine application is part of the system image. It is installed as binary package into `/opt/middleby_baseline`. A symbolic link `/opt/middleby` is created that points to `/opt/middleby_baseline`. Furthermore, the system image also contains the installation scripts in `/opt/middleby_setup_scripts`. All systemd service files are installed int `/etc/systemd/system` to be executed when the `multi-user.target` runs.

The application is started after the Weston compositor has been started as user `synesso`. The application itself also runs as user `synesso`.

On launch the `sbengine` backend process is granted `CAP_SYS_TIME` capabilities to allow it to set the system time. The system time is meant to be set in local time with timezone and daylight saving time flag. The hardware clock of the system is automatically synchronized by system processes and the kernel. The kernel restores the system time automatically from the hardware clock on boot. If network is configured and a standard Internet time server is reachable the system time is automatically synchronized via NTP. The timezone, if set, is taken into consideration. The hardware clock is maintained in UTC.
