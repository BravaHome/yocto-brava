![ibeeto](artwork/ibeeto-logo.png "ibeeto")


# Middleby MLUI Yocto Project Build System

Yocto Project meta data and build environment setup for Middleby MLUI board.

## History

Version     | Date       | Author             | Description
----------- | ---------- | ------------------ | -------------------------------------------
1.0         | 2021/08/13 | Rudolf J Streif    | Initial Version
1.1         | 2022/03/15 | Rudolf J Streif    | Added block device automount
1.2         | 2022/03/16 | Rudolf J Streif    | Added hardware clock
1.3         | 2022/03/23 | Rudolf J Streif    | Modified hardware clock section
1.4         | 2022/07/06 | Rudolf J Streif    | Added Bluetooth section
1.5         | 2023/06/06 | Rudolf J Streif    | Added RAUC section



## User Setup

For the Yocto Build System to be able to download source repositories from this Gitlab server you need to add SSH keys to your Gitlab user account. Below you find basic instructions on how to generate a key pair, add it to your Gitlab user account, and configure your Linux workstation. If you are interested in more detailed instructions, refer to the [Gitlab Documentation](https://docs.gitlab.com/ee/ssh/).


### Generating a new SSH key pair

We recommend that you use ED25519 keys as they are more secure and use a faster algorithm. A modern Linux distribution should be able to create ED25519 keys.

1. Open a terminal window on your Linux workstation.

2. Create your ED25519 key pair with:

```
$ ssh-keygen -t ed25519 -C "email@example.com"
```

If you cannot create an ED25519 key pair or prefer RSA keys create you RSA key pair with

```
$ ssh-keygen -t rsa -b 4096 -C "email@example.com"
```
The `-t` flag determines the key type; the `-b` flag specifies the key length for RSA keys; and the `-C` flag adds a comment to your key allowing you to distinguish your keys.

3. Next, ssh-keygen will prompt you to input a file path to save your SSH key pair to. Either accept the suggestion or enter your own file name. It is recommended that you store you keys in `~/.ssh`.

4. Now ssh-keygen will ask you if you want to secure your key with a password. This will cause ssh to ask for the password when the key is used. It is best practice to use a password but it is not required. If you don't want to use a password simply press `Enter` twice.


### Adding ibeeto Gitlab to your SSH configuration

To tell ssh what key to use when accessing the ibeeto Gitlab server you need to add it your `~/.ssh/config` file. Add the following lines to the file (replace <keyfile> with the name of your keyfile):

```
Host gitlab.ibeeto.com
    Preferredauthentications publickey
    IdentityFile ~/.ssh/<keyfile>
```


### Adding your SSH key to ssh-agent

ssh-agent is a service that stores and provides keys to ssh. If you are using a key without password protection and have added it to your ssh configuration you do not need to setup ssh-agent. If your key is password protected then it is recommended that you use ssh-agent otherwise ssh through the Yocto Project build system will ask you to enter the password for you key every time it needs to access the ibeeto Gitlab server.

1. Start the ssh-agent in the background:

```
$ eval "$(ssh-agent -s)"
```

To start ssh-agent each time you log into your workstation add the above line to your `~/.bashrc` file.

2. Add your SSH private key to ssh-agent (replace <keyfile> with the name of your keyfile):

```
$ ssh-add ~./ssh/<keyfile>
```


### Adding your SSH key to your ibeeto Gitlab account

1. Copy your **public** SSH key to the clipboard. You can open the file with the public key (`~/.ssh/<keyfile>.pub`) in a graphical editor and selecting and copying the entire contents to the clip board. Alternatively you can use `xclip` to copy the file to the clipboard (xclip package must be installed on your workstation):

```
$ xclip -sel clip < ~/.ssh/<keyfile>.pub
```

2. Add your public SSH key to your ibeeto GitLab account by clicking your avatar in the upper right corner and selecting **Preferences**. From there on, navigate to **SSH Keys** and paste your public key in the box in the **Key** section. If you created the key with a comment, this will appear under **Title**. If not, give your key an identifiable title, and click Add key.

>>>
If you manually copied your public SSH key from a graphical editor ensure that you copied the entire key starting with `ssh-ed25519` (or `ssh-rsa`) and ending with your email address.
>>>


### Testing your SSH connection

To test the SSH connection with your keys simply type:

```
$ ssh -T git@gitlab.ibeeto.com
```

If your keys are set up correctly then the ibeeto GitLab server responds with:

```
Welcome to GitLab, @<username>!
```

The placeholder <username> is replaced with your actual user name.



## Workstation Setup

The Yocto Project build system requires certain software packages to be installed on your workstation. The list and name of the packages depends on the Linux distribution you are using on your workstation.

### CentOS

For CentOS run the following commands:

```
$ sudo yum install -y epel-release
$ sudo yum makecache
$ sudo yum install -y gawk make wget tar bzip2 gzip python unzip perl patch \
     diffutils diffstat git cpp gcc gcc-c++ glibc-devel texinfo chrpath socat \
     perl-Data-Dumper perl-Text-ParseWords perl-Thread-Queue python34-pip xz \
     which SDL-devel xterm libstdc++-static
$ sudo yum install -y make docbook-style-dsssl docbook-style-xsl \
     docbook-dtds docbook-utils fop libxslt dblatex xmlto
$ sudo pip3 install GitPython jinja2
```

### Debian and Ubuntu

For Debian and Ubuntu run the following commands:

```
$ sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib \
  build-essential chrpath socat cpio python python3 python3-pip python3-pexpect \
  xz-utils debianutils iputils-ping python3-git python3-jinja2 libegl1-mesa libsdl1.2-dev \
  pylint3 xterm
$ sudo apt-get install make xsltproc docbook-utils fop dblatex xmlto
```

### Fedora

For Fedora run the following commands:

```
$ sudo dnf install gawk make wget tar bzip2 gzip python3 unzip perl patch \
  diffutils diffstat git cpp gcc gcc-c++ glibc-devel texinfo chrpath \
  ccache perl-Data-Dumper perl-Text-ParseWords perl-Thread-Queue perl-bignum socat \
  python3-pexpect findutils which file cpio python python3-pip xz python3-GitPython \
  python3-jinja2 SDL-devel xterm libstdc++-static
$ sudo dnf install docbook-style-dsssl docbook-style-xsl \
  docbook-dtds docbook-utils fop libxslt dblatex xmlto
```

### OpenSuSE

For OpenSuSE run the following commands:

```
$ sudo zypper install python gcc gcc-c++ git chrpath make wget python-xml \
   diffstat makeinfo python-curses patch socat python3 python3-curses tar python3-pip \
   python3-pexpect xz which python3-Jinja2 Mesa-libEGL1
$ sudo zypper install dblatex xmlto
$ sudo pip3 install GitPython libSDL-devel xterm
```

## Cloning the Repository

In you home directory, or another directory of your choice, create a `middleby` subdirecotry and clone the repository:

```
$ mkdir -p ~/middleby
$ cd ~/middleby
$ git clone git@gitlab.ibeeto.com:middleby/mlui.git
$ cd mlui
$ git submodule update --init --recursive
```

After you cloned the repository once you can update it with:

```
$ cd ~/middleby/mlui
$ git pull
```


## Building

We have added a build script to simplify kicking off a build providing interactive choices for the target system, the target image and the target image type. This is the most convenient way to get started quickly.

If you are familiar with the Yocto Project you can of course directly source your build environment to your liking and run BitBake.

Either way, you need about 100 GB of storage on your workstation to hold the source downloads and the build artifacts.

### Build Configuration

A build is configured by providing a target system or target machine, a target image and a target image type. Currently these choices are available:

* Target Machine
  * `mlui-lcd7` - MLUI with 7" LCD touchscreen display
  * `mlui-lcd5` - MLUI with 5" LCD touchscreen display

* Target Image
  * `core-image-mlui` - Base image to test the board and BSP
  * `core-image-brava` - Image for Middleby Brava division
  * `core-image-synesso` - Image for Middleby Synesso division

* Target Image Type
  * `production` - Image for production deployment
  * `development` - Image for development (additional tools for development)
  * `sdk` - SDK installer for host system based on `development` image

The build script walks you through the options by presenting selections for machine, image and image type. If you choose to use BitBake directly you have to set the machine by setting the `MACHINE` variable in `conf/local.conf` of your build environment and provide the target image to build. Please see below in section [Directly Building with BitBake](README.md#Directly_Building_with_BitBake).

For details on how to build and work with development images please refer to section [Software Development Kit (SDK)](README.md#Software_Development_Kit_(SDK)).

### Building with the Build Script

The build script provides an interactive menu for configuring the build. Follow the instructions to start building with the build script.

1. First change to the directory where you have cloned the repository into:

```
$ cd ~/middleby/mlui
```

2. Invoke the build script:

```
$ tools/build.sh
```

The script presents you with an interactive menu:

```
BUILDING DEVICE IMAGE
=====================

1) mlui-lcd7
2) mlui-lcd5
3) mlui-lcd7-usmp-t070
Please choose the machine to build for: 1

1) core-image-mlui
2) core-image-brava
3) core-image-synesso
Please choose the image to build: 2

1) production
2) development
3) sdk
Please choose the image type to build: 2

[...]
```

Once the build has finished you can find the images for the target board in the directory `build/tmp/deploy/images/<machine>`.

### Directly Building with BitBake

Follow the instructions below to source a build environment and to kick off your build.

1. First change to the directory where you have cloned the repository into:

```
$ cd ~/middleby/mlui
```

2. Source the build environment:

```
$ source poky/oe-init-build-env build
```

3. Start the build:

```
$ MACHINE="<machine>" bitbake -k core-image-<image>
```

Where `<machine>` is one of the machine configurations and `<image>` is one of the target images.

Once the build has finished you can find the images for the target board in the directory `build/tmp/deploy/images/<machine>`.



## Booting the Target

### Booting from SD Card

To boot the target from a SD card you need a card with at least 4 GB capacity. As it is a boot device the entire drive will be reformatted and any data on it will be lost.

1. Insert your SD card into a SD card reader connected to your workstation and determine the device:

```
$ dmesg
[1627319.341421] sd 7:0:0:1: [sdd] 30228480 512-byte logical blocks: (15.5 GB/14.4 GiB)
[1627319.345545]  sdd: sdd1 sdd2
```

Look for a line similar to `[1627319.341421] sd 7:0:0:1: [sdd] 30228480 512-byte logical blocks: (15.5 GB/14.4 GiB)`. The three letters in brackets `[sdd]` is the device. Dependent on how many disks your workstation has connected the device might be different. For a workstation with one system disk it typically is `sdd`.

2. Copy the image to the device:

```
$ cd ~/middleby/mlui
$ sudo tools/mkbootdisk.sh
```

The script guides you through the process. Make sure that you specify the correct device, as determined by the first step.

3. Insert the SD card into the SD socket on the target and start the target. The target should detect the SD card a bootable device and first boot the bootloader u-boot and then Linux.

If you have a serial console connected it shows the boot log:

```
DRAM: 1024 MiB                          
Trying to boot from MMC1                
alloc space exhausted                   
                                        

U-Boot 2020.07-g2f5fbb5 (May 18 2021 - 18:22:35 +0000) Allwinner Technology

CPU:   Allwinner A64 (SUN50I)
Model: Middleby MLUI
DRAM:  1 GiB
MMC:   mmc@1c0f000: 0, mmc@1c10000: 1
Loading Environment from FAT... Card did not respond to voltage select!
Warning: HDMI PHY init timeout!
Warning: HDMI PHY init timeout!
In:    serial
Out:   serial
Err:   serial
Net:   No ethernet found.
starting USB...
Bus usb@1c1a000: USB EHCI 1.00
Bus usb@1c1a400: USB OHCI 1.0
Bus usb@1c1b000: USB EHCI 1.00
Bus usb@1c1b400: USB OHCI 1.0
scanning bus usb@1c1a000 for devices... 1 USB Device(s) found
scanning bus usb@1c1a400 for devices... 1 USB Device(s) found
scanning bus usb@1c1b000 for devices... 1 USB Device(s) found
scanning bus usb@1c1b400 for devices... 1 USB Device(s) found
       scanning usb for storage devices... 0 Storage Device(s) found
Hit any key to stop autoboot:  0 
switch to partitions #0, OK
mmc0 is current device
Scanning mmc 0:1...
Found U-Boot script /boot.scr
495 bytes read in 1 ms (483.4 KiB/s)
## Executing script at 4fc00000
30555 bytes read in 4 ms (7.3 MiB/s)
28318208 bytes read in 1363 ms (19.8 MiB/s)
## Flattened Device Tree blob at 4fa00000
   Booting using the fdt blob at 0x4fa00000
EHCI failed to shut down host controller.
   Loading Device Tree to 0000000049ff5000, end 0000000049fff75a ... OK

Starting kernel ...

[    0.000000] Booting Linux on physical CPU 0x0000000000 [0x410fd034]
[    0.000000] Linux version 5.10.2 (oe-user@oe-host) (aarch64-middleby-linux-gcc (GCC) 10.2.0, GNU ld (GNU Binutils) 2.35.1) #1 SMP PREEMPT Thu Aug 5 15:54:1
[    0.000000] Machine model: Middleby MLUI

[...]

mlui login:
```

### Logging In

#### Logging in via serial console

To log in using the serial console you need to connect a FTDI 3.3V USB-to-UART adapter to the serial console port on the device. You need to make sure that your adapter is a 3.3V version. Using a 5V version will destroy the SoC. You can find these adapters on Amazon: https://www.amazon.com/gp/product/B06ZYPLFNB. These adapters have the following pinout:

1. Black - GND
2. Brown - CTS
3. Red - 5V
4. Orange - TxD
5. Yellow - RxD
6. Green - RTS

**Note:** Make sure that you use a 3.3V FTDI USB-to-UART adapater. Using a 5V version will destroy the SoC. You can use a voltmeter to verify that you have the correct adapter. On a 3.3V adapter CTS, TxD, RxD and RTS will measure 3.3V against GND.

On the MLUI board the console UART is routed to the `DEBUG1' connector. With the board oriented so that the silk screen printing is not upside-down (SD card connector is to the right), the pinout of the connector is from bottom to top:

1. RXD
2. TXD
3. GND

Connect the pins on the cable to the appropriate pins on the serial console connector on the device and then plug the USB A connector into a USB port on your development system. To determine to what serial device the adapter is attached to type `dmesg` in a terminal window of your development system:

```
$ dmesg
[...]
[208661.324842] usb 7-1.4.4: New USB device found, idVendor=0403, idProduct=6001, bcdDevice= 6.00
[208661.324846] usb 7-1.4.4: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[208661.324848] usb 7-1.4.4: Product: TTL232R-3V3
[208661.324850] usb 7-1.4.4: Manufacturer: FTDI
[208661.324851] usb 7-1.4.4: SerialNumber: FTAJYUQ8
[208661.327902] ftdi_sio 7-1.4.4:1.0: FTDI USB Serial Device converter detected
[208661.327923] usb 7-1.4.4: Detected FT232RL
[208661.331988] usb 7-1.4.4: FTDI USB Serial Device converter now attached to ttyUSB0
```

In this example the adapter is attached to `/dev/ttyUSB0`. Use a serial terminal program such as `screen` or `minicom` to connect to the device:

```
$ screen /dev/ttyUSB0 115200
mlui login: root
root@mlui:~#
```

There is no password for `root`.

To exit `screen` type `CTRL-A K` and then `y`.

#### Logging in via SSH

The device is configured to obtain an IP address via DHCP using the wired Ethernet port. The device identifies itself as `mlui` to the DHCP server. To determine if the server resolves the address correctly you can ping the device:

```
$ ping mlui
PING mlui (192.168.4.167) 56(84) bytes of data.
64 bytes from 192.168.4.167: icmp_seq=1 ttl=64 time=1.46 ms
64 bytes from 192.168.4.167: icmp_seq=2 ttl=64 time=1.44 ms
64 bytes from 192.168.4.167: icmp_seq=3 ttl=64 time=1.47 ms
64 bytes from 192.168.4.167: icmp_seq=4 ttl=64 time=1.51 ms
^C
--- 192.168.4.167 ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 3005ms
rtt min/avg/max/mdev = 1.442/1.470/1.508/0.023 ms
```

If pinging the device does not work correctly because of the address resolution you might need to look at your networking equipment's DHCP list.

The SSH server on the device is configured to only allow login using RSA keys. Keys for the `root` user have been pre-configured at build time. For logging in with ssh use

```
$ ssh -o IdentitiesOnly=yes -o StrictHostKeyChecking=no -i build/tmp/deploy/keys/root-sshkey root@<target-ip>
```

For details on SSH configuration for the device refer to section **SSH Configuration** below.



## Build System Layout

The build system is entirely self-contained in a single directory `mlui` that contains all the necessary Yocto Project tools and layers as well as the build environment itself.

The subdirectories are:

* `build`: Build environment
* `meta-middleby-bsp`: Custom Middleby BSP layer for the MLUI device
* `meta-middleby-distro`: Custom Middleby distribution layer (see below)
* `meta-openembedded`: Metadata layer for additional software packages
* `meta-qt5`: Metadata layer for Qt5
* `meta-sunxi`: Metadata layer for Allwinner SoC support
* `poky`: Yocto project core
* `tools`: Integration and convenience tools and scripts
* `README.md`: Instructions (this file)
* `README-SDK.md`: Instructions for the SDK


### Custom Layers

#### meta-middleby-bsp

This BSP layer is providing adaptations for the Middleby MLUI hardware. It defines the machines for the various MLUI configurations. Additionally it contains patches and configuration for the bootloader u-boot and other hardware-specific adaptations.


#### meta-middleby-distro

This layer is the Middleby distribution layer. It provides the settings for the `mlui` distribution (`conf/distro/mlui.conf`), recipes for software adaptations as well as the definitions for the target images. For example, the recipe for the mlui development target image is `recipes-core/images/core-image-mlui-dev.bb`.

The layer is meant to be a container for all of the application recipes for the Middleby software. For this purpose the layer has a subdirectory `recipes-application` in which the recipe meta-data for the applications is organized into application-specific subdirectories. See [Application Developement](README.md#Application_Development) below for details.



## System Configuration

### Users and Passwords

The build system creates all the default users for system services etc. That of course also includes the login for the `root` user. If you want to change the password for the `root` user edit the variable `ROOT_PASSWORD` in the file `build/conf/local.conf`.

You can add additional users to your system by editing the variable `NEWUSERS` in the file `build/conf/local.conf`. The variable contains a space-delimited list of `username:password:groups`. `groups` is a comma-delimited list of additional groups to add the user to.

System users are created by setting the password to `<system>` in the `username:password:groups` list. A system user has no home directory and login is disabled.

A daemon user is similar to a system user but has a home directory. To create a daemon user set the password to `<daemon>` in the `username:password:groups` list.


### User Automatic Login

A non-priviledged user account can be set up for the system to automatically log and run commands. This user is set with the variable `AUTO_LOGIN_USER` in the file `build/conf/local.conf`. The user account specified by this variable must be one of the user accounts configured with the variable `NEWUSERS`.

If after logging in the system needs to run any commands they can be added to the file `profile.<user>` where `<user>` is the name of the user configured for automatic login. The file resides in `meta-middleby-distro/recipes-core/images/files`. The content of the file is appended verbatim to the `.profile` file in the user's home directory.


### SSH Configuration

The build system can configure SSH login with RSA key pairs for users. You can control for what user accounts you want to create SSH access with keys by editing the variable `SSH_USERS` in the file `build/conf/local.conf`. The variable is a space-delimited list of user names. If you add a user name to the list you need to make sure that is has previously been created (see `NEWUSERS` above).

The build system stores the key pairs for each user in the directory `build/tmp/deploy/keys`. The key files are named `<username>-sshkey` for the private key and `<username>-sshkey.pub` for the public key.

The OpenSSH server is installed on the target system. On an incoming connection on port 22 systemd launches the OpenSSH server automatically. You can test the SSH connection to the target for the root user with:

```
$ ssh -o IdentitiesOnly=yes -o StrictHostKeyChecking=no -i build/tmp/deploy/keys/<user>-sshkey <user>@<target-ip>
```

Replace `<target-ip>` with the actual IP address of your target and `<user>` with the actual user name.


### Network configuration

#### Wireless Networks

Connections to wireless networks are handled by *Network Manager*. You can use the Network Manager command line interface *nmcli* to manage connections to wireless networks.

The MLUI does not have built-in WiFi. Ether a USB WiFi adapter needs to be plugged into a USB port or a supported m.2 WiFi module needs to be installed.

To obtain a list of wireless networks in range use:

```
root@mlui:~# nmcli d wifi
IN-USE  SSID                   MODE   CHAN  RATE        SIGNAL  BARS  SECURITY
        IBEETO-HQ-ADMIN        Infra  1     405 Mbit/s  95      ****  WPA2     
*       IBEETO-HQ-ENG          Infra  1     405 Mbit/s  94      ****  WPA2     
        IBEETO-HQ-COMM         Infra  1     405 Mbit/s  89      ****  WPA2       
        IBEETO-HQ-GUEST        Infra  1     405 Mbit/s  89      ****  --       
        IBEETO-HQ-ADMIN        Infra  10    405 Mbit/s  70      ***   WPA2     
        IBEETO-HQ-ENG          Infra  10    405 Mbit/s  69      ***   WPA2       
        IBEETO-HQ-COMM         Infra  10    405 Mbit/s  67      ***   WPA2     
        IBEETO-HQ-GUEST        Infra  10    405 Mbit/s  66      ***   --     
        IBEETO-HQ-GUEST        Infra  6     270 Mbit/s  55      **    --       
        IBEETO-HQ-ENG          Infra  6     270 Mbit/s  54      **    WPA2     
        IBEETO-HQ-ADMIN        Infra  6     270 Mbit/s  50      **    WPA2     
        IBEETO-HQ-COMM         Infra  6     270 Mbit/s  32      **    WPA2     
```

The wireless networks are listed in order of signal strength. If the device is currently connected to one or more wireless networks the *IN-USE* column shows a *\** next to the connected wireless network.

To connect to a wireless network use `nmcli d wifi connect <ssid> password <secret>`:

```
root@mlui:~# nmcli d wifi connect IBEETO-HQ-ENG password <secret>
IPv6: ADDRCONF(NETDEV_UP): mlan0: link is not ready
mwifiex_sdio mmc0:0001:1: info: trying to associate to 'IBEETO-HQ-ENG' bssid f0:9f:c2:3a:ed:df
mwifiex_sdio mmc0:0001:1: info: associated to bssid f0:9f:c2:3a:ed:df successfully
IPv6: ADDRCONF(NETDEV_CHANGE): mlan0: link becomes ready
mwifiex_sdio mmc0:0001:1: CMD_RESP: cmd 0x10f error, result=0x2
Device 'mlan0' successfully activated with '2e5eb25b-38cd-432e-99da-c22dd1f7e291'.
```

Network Manager stores connection information in connection files named `<ssid>.nmconnection` in the directory `/etc/NetworkManager/system-connections`. The password is stored in clear text in the file. Hence it is mandatory that the files are owned by *root* and the access permissions are set to *0600*. When using nmcli ownership and permissions are set correctly. If you create connection files manually using an editor then you have to set ownership and permissions accordingly. Network Manager ignores connection files with incorrect ownership and/or permissions.

If you mistype a password and the connection fails Network Manager still creates the connection file with the incorrect password. To correct it you have to remove the connection file first because Network Manager ignores the password provided with the command line when a connection file for a connection is present. You can use nmcli to delete a connection: `nmcli c delete <connection>` where *<connection>* is the ssid of the wireless network.

#### Bluetooth

Bluetooth for the MLUI is provided by the same Embedded Artists 1MW module that also provides WiFi connectivity.

The module attaches to the BlueZ Bluetooth stack via `hci0` on startup:

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

### Boot Media Configuration and Creation

The build system automatically creates complete images for the boot medium. The boot medium can either be a SD card that is plugged into the SD card socket of the MLUI or the on-board eMMC. The storage space on the boot medium is divided into several partitions. The partition layout is defined by configuration files in the directory `meta-middleby-bsp/wic`.

These files are processed by the  *Openembedded Image Creator (WIC)*. What configuration file WIC uses to create the boot medium is determined by the variable `WKS_FILE`. This variable is commonly set in a machine configuration file. However, it can also be set in `build/conf/local.conf`. For example `brava-sdcard-image.wks.in` contains the configuration for Brava MLUI systems:

```
# short-description: Create SD card / eMMC image for Brava configuration
# long-description:
# Create image for SD card or eMMC with Brava partition layout.
#
# The disk layout used is:
#  - --------------------------- ------ --------- ---------- --------- --------- ------ 
# | | u-boot-sunxi-with-spl.bin | boot | kernelA | kernel B | rootfsA | rootfsB | data |
#  - --------------------------- ------ --------- ---------- --------- --------- ------
# ^ ^                           ^      ^         ^          ^         ^         ^      ^
# | |                           |      |         |          |         |         |      |
# 0 |                         2MiB   +40MiB    +32MiB     +32MiB    +1GiB     +1GiB  +5GiB
#   8KiB 
#
# The layout supports A/B banking for software updates. The boot partition contains a u-boot
# script that can be used to either boot from bank A (kernelA/rootfsA) or bank B (kernelB/rootfsB).
#

part u-boot --source rawcopy --sourceparams="file=u-boot-sunxi-with-spl.bin" --no-table --align 8
part /boot  --source bootimg-partition --use-uuid --fstype=vfat --label boot    --align 2048 --fixed-size 40 --active
part        --source bootimg-partition --use-uuid --fstype=ext4 --label kernelA --align 2048 --fixed-size 32
part        --source bootimg-partition --use-uuid --fstype=ext4 --label kernelB --align 2048 --fixed-size 32
part /      --source rootfs            --use-uuid --fstype=ext4 --label rootfsA --align 2048 --size 1000
part                                   --use-uuid --fstype=ext4 --label rootfsB --align 2048 --size 1000
part /brava                            --use-uuid --fstype=ext4 --label data    --align 2048 --size 5000

bootloader --ptable msdos
```

The WIC configuration file as also referred to as a *kickstart file*. It has its origins in the RedHat Linux operating system installation automation. Details on the file format and the OE/YP specific extensions can be found at [OpenEmbedded Kickstart Reference](https://docs.yoctoproject.org/ref-manual/kickstart.html).

A partition is defined by the `part` keyword followed by a list of parameters: 

* `<mount point>` - the path of the mount point, or no entry if the partition is not mounted (`u-boot` is reserved for the boot loader and not a mount point)
* `--source` - the name of the source plugin:
  * `rawcopy` - plugin to byte-copy image to the partition
  * `bootimg-partition` - plugin to create a boot partition with specific content
  * `rootfs` - plugin to create a root file system partition
* `--use-uuid` - create a random uuid to identify the partition for mounting independent of device name
* `--fstype <file system type>` - file system type
* `--label <partition label>` - partition label
* `--align <alignment boundary>` - partion block alignment boundary in kiB
* `--fixed-size <size>` - exact size of the partition in MiB
* `--size <size>` - minimum size of the partition in MiB, WIC will increase the partition size if the content is larger than `<size>`

The `bootloader` entry defines the partition table scheme. In this case the scheme is `msdos` (Master Boot Record (MBR)).

The u-boot image it located in the `u-boot` partition which must exactly start at the 8kiB boundary otherwise the A64 will not be able to correctly load and execute it. This partition also must not have a file system as the A64's primary image loader (PIL) cannot read file systems.

If a *mount point* has been providen, WIC adds the partition to `/etc/fstab` automatically. Mount options for the file system is set to *defaults*. You can specify different mount options by adding the `--fsoptions "<mount options>"` to the list of parameters. `<mount options>` is a free-from string and must be enclosed in quotes. WIC copies the string verbatim into `/etc/fstab`.

The partition scheme above supports A/B banking for software updates. There are two kernel and two root file system partitions:

* Bank A: kernelA and rootfsA
* Bank B: kernelB and rootfsB

The `boot` partition contains a u-boot script that is used to identify from which device, SD card or eMMC, the system is booting and to switch between the two boot banks. The boot script to boot from bank A looks as follows:

```
# Brava u-boot boot script for MLUI

# Kernel partition for bank A is 3 and for bank B is 4
kernelpart=3

# Root file system partition for bank A is 5 and for bank B is 6
rootpart=5

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
```

The boot script essentially contains commands for u-boot interpreter. The build system processes the file and copies it to the boot partition.



## Hardware configuration

### I2C Bus

The Linux i2c-tools package is installed on the system and the tools can be used to examine the i2c buses, probe devices as well as read and write registers of connected i2c devices.

There is only one active i2c bus on the system which is accessible through the `/dev/i2c-3` device:

```
root@mlui:~# i2cdetect -l
i2c-1   i2c             mv64xxx_i2c adapter                     I2C adapter
i2c-0   i2c             mv64xxx_i2c adapter                     I2C adapter
```

To list the devices on the `i2c-0` bus:

```
root@mlui:~# i2cdetect 0
I will probe file /dev/i2c-0.
I will probe address range 0x03-0x77.
Continue? [Y/n] 
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: UU -- -- -- -- -- -- -- -- -- -- -- UU -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- 48 -- -- -- -- -- -- -- 
50: -- UU -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --                         
```

The connected devices are at these addresses:

* 0x20 - NXP PCA9574 GPIO extender
* 0x2c - Microchip USB2422 hub
* 0x48 - ST Microelectronics STLM75 temperature sensor
* 0x51 - MicroCrystal RV-8263 RTC (only with MLUI RTC sub-board)

List of devices connected to the `i2c-1` bus:

```
root@mlui:~# i2cdetect 1
WARNING! This program can confuse your I2C bus, cause data loss and worse!
I will probe file /dev/i2c-1.
I will probe address range 0x03-0x77.
Continue? [Y/n] 
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- 40 -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --                         
```

The connected devices are at these addresses:

* 0x4a - Atmel Maxtouch touch screen controller (only with Orient Displays)

When the `i2c-detect` tool shows `UU` instead of the address it indicates that device probing was skipped because a kernel driver is currently controlling the device.



## Root File System Configuration

The recipes in `meta-middleby-distro\recipes-core\images` define various configurations for the target root file system by selecting what software packages are installed.

The recipes for the images in the directory are:

* `core-image-brava-dev.bb`: Development image for Middleby Brava division
* `core-image-brava-prd.bb`: Production image for Middleby Brava division
* `core-image-synesso-dev.bb`: Development image for Middleby Synesso division
* `core-image-synesso-prd.bb`: Production image for Middleby Synesso division
* `core-image-mlui-dev.bb`: Test development image
* `core-image-mlui-prd.bb`: Test production image

These are the image recipes that are built by the Yocto Project build system either by using the build script or by directly invoking Bitbake with the image name.

Production and development images include a base configuration file that contains settings and software packages that are common between them:

* `core-image-brava-base.inc`: Common configuration for Brava division images
* `core-image-synesso-base.inc` : Common configuration for Synesso division images

All base configurations include the file `core-image-mlui-base.inc` which defines the core image configuration.

To add software packages to the development and production images use the following variables that are defined by the image recipes:

* Image `core-image-brava-dev.bb`: `IMAGE_INSTALL_BRAVA_DEV`
* Image `core-image-brava-prd.bb`: `IMAGE_INSTALL_BRAVA_PRD`
* Image `core-image-synesso-dev.bb`: `IMAGE_INSTALL_SYNESSO_DEV`
* Image `core-image-synesso-prd.bb`: `IMAGE_INSTALL_SYNESSO_PRD`

To software packages to development and production images alike you can use these variables defined by the include files:

* Include file `core-image-brava-base.inc`: IMAGE_INSTALL_BRAVA
* Include file `core-image-synesso-base.inc`: IMAGE_INSTALL_SYNESSO



## Package Management

The build system assembles the root file system using the Red Hat Package Manager (RPM). The RPM package manager is also installed on the target's root file system for package maintenance.

### Querying Installed Packages

To query the package database for all installed packages use:

```
root@mlui:~# rpm -qa
gstreamer1.0-plugins-bad-smoothstreaming-1.16.3-r0.cortexa53_crypto
kernel-devicetree-5.10.2-r0.mlui_lcd7
python3-pygobject-3.36.1-r0.cortexa53_crypto
python3-netserver-3.8.5-r0.cortexa53_crypto
libc6-extra-nss-2.32-r0.cortexa53_crypto
alsa-utils-aseqnet-1.2.3-r0.cortexa53_crypto
perl-module-file-stat-5.32.0-r0.cortexa53_crypto
perl-module-file-spec-win32-5.32.0-r0.cortexa53_crypto
kernel-module-rc-beelink-gs1-5.10.2-5.10.2-r0.mlui_lcd7
intltool-0.51.0-r0.cortexa53_crypto
libmount1-2.36-r0.cortexa53_crypto
gstreamer1.0-plugins-good-audiofx-1.16.3-r0.cortexa53_crypto
kernel-module-nf-defrag-ipv4-5.10.2-5.10.2-r0.mlui_lcd7
man-db-dev-2.9.3-r1.cortexa53_crypto

[output shortened]
```

The packages are listed in order of the package hashes. To list them alphabetically:

```
root@mlui:~# rpm -qa | sort
acl-2.2.53-r0.cortexa53_crypto
acl-dev-2.2.53-r0.cortexa53_crypto
adwaita-icon-theme-symbolic-3.36.1-r0.noarch
alsa-conf-1.2.3.2-r0.cortexa53_crypto
alsa-state-0.2.0-r5.cortexa53_crypto
alsa-state-dev-0.2.0-r5.cortexa53_crypto
alsa-states-0.2.0-r5.cortexa53_crypto
alsa-tools-1.2.2-r0.cortexa53_crypto
alsa-tools-dev-1.2.2-r0.cortexa53_crypto
alsa-topology-conf-1.2.3-r0.noarch
alsa-ucm-conf-1.2.3-r0.noarch
alsa-utils-1.2.3-r0.cortexa53_crypto
alsa-utils-aconnect-1.2.3-r0.cortexa53_crypto
alsa-utils-alsactl-1.2.3-r0.cortexa53_crypto

[output shortened]
```

To look for a package containing a certain string:

```
root@mlui:~# rpm -qa | grep base | sort
base-files-3.0.14-r89.mlui_lcd7
base-files-dev-3.0.14-r89.mlui_lcd7
base-passwd-3.5.29-r0.cortexa53_crypto
base-passwd-dev-3.5.29-r0.cortexa53_crypto
gstreamer1.0-plugins-base-1.16.3-r0.cortexa53_crypto
gstreamer1.0-plugins-base-adder-1.16.3-r0.cortexa53_crypto
gstreamer1.0-plugins-base-alsa-1.16.3-r0.cortexa53_crypto
gstreamer1.0-plugins-base-app-1.16.3-r0.cortexa53_crypto
gstreamer1.0-plugins-base-apps-1.16.3-r0.cortexa53_crypto
gstreamer1.0-plugins-base-audioconvert-1.16.3-r0.cortexa53_crypto
gstreamer1.0-plugins-base-audiomixer-1.16.3-r0.cortexa53_crypto
gstreamer1.0-plugins-base-audiorate-1.16.3-r0.cortexa53_crypto
gstreamer1.0-plugins-base-audioresample-1.16.3-r0.cortexa53_crypto

[output shortened]
```


### Installing Packages

Packages can be installed locally using their RPM package file (ending in .rpm). A package file can be copied to the target using sftp. The RPM package manager does not download from a remote server. In fact, it does not have the capability to do so.

For example, to install the `xz-doc` package use sftp to copy it over the network. The package file is `build/tmp/deploy/rpm/cortexa53_crypto/xz-doc-5.2.5-r0.cortexa53_crypto.rpm`.

On the target system execute:

```
root@mlui:~# rpm -qa | grep xz
xz-dev-5.2.5-r0.cortexa53_crypto
xz-5.2.5-r0.cortexa53_crypto
root@mlui:~# rpm -i xz-doc-5.2.5-r0.cortexa53_crypto.rpm
root@mlui:~# rpm -qa | grep xz
xz-dev-5.2.5-r0.cortexa53_crypto
xz-doc-5.2.5-r0.cortexa53_crypto
xz-5.2.5-r0.cortexa53_crypto
```

For illustration purposes the first command lists all packages that have `xz` in their name. The second command installs the package and the third validates the installation.


### Removing Installed Packages

Installed packages can be removed from the system using the `rpm -e <packagename>` syntax:

```
root@mlui:~# rpm -qa | grep xz
xz-dev-5.2.5-r0.cortexa53_crypto
xz-doc-5.2.5-r0.cortexa53_crypto
xz-5.2.5-r0.cortexa53_crypto
root@mlui:~# rpm -e xz-doc                         
root@mlui:~# rpm -qa | xz
xz-dev-5.2.5-r0.cortexa53_crypto
xz-5.2.5-r0.cortexa53_crypto
```

### Upgrading Installed Packages

Installed packages can be upgraded using the `rpm -U <packagename>` syntax:

```
root@mlui:~# rpm -U xz-doc-5.2.5-r0.cortexa53_crypto
root@mlui:~# rpm -qa | grep xz
xz-dev-5.2.5-r0.cortexa53_crypto
xz-doc-5.2.5-r0.cortexa53_crypto
xz-5.2.5-r0.cortexa53_crypto
```

If the package is not yet installed, upgrading works the same as installing. If an older version of the package is installed, upgrading installs the new version provided with the package file. If the same version is already installed, upgrading is aborted. If a newer version is already installed, in which case it would be a downgrade, `rpm` aborts and asks to be restarted with the `-f` option to force the downgrade.



## System Configuration

### Media Automounting

The system automatically mounts media block devices e.g. USB thumb drives.

The mount points are: /run/media/sd`\<drive>\<partition>`

The first `\<drive>` plugged into (either the USB-A or the micro USB port) is `a`, the second drive plugged in is `b`, etc.

The first `\<partition>` of a drive is 1, etc. However, only partitions with valid file systems are mounted. Valid file system types are ext2, ext3, ext4, fat, vfat.

### Hardware Clocks

Hardware clocks, also commonly referred to as real-time clocks (RTC), keep the "wall clock" time while the system is powered off and the kernel cannot maintain the system time. Hardware clock are powered from a battery so they keep running while the system is powered down. When the system boots the system time gets restored from the hardware clock.

The MLUI has a built-in hardware clock that is powered by a rechargeable ML1220 3V Lithium battery that is soldered to the MLUI board. When the MLUI is powered its circuits charges the battery. When the MLUI is not powered the battery powers the RTC. The built-in RTC draws on average 60uA. The battery capacity is 18mAh. This results in about 300 hours (12.5 days) of time keeping while the MLUI is powered off. That is sufficient for applications where the MLUI is powered on most of the time and only powered-off for short periods.

For applications that require longer periods of the MLUI being powered down, the MLUI can be outfitted with a separate low-power RTC board that directly plugs into the I2C extension connector J5. The low-power RTC chip draws 190nA from the CR2032 Lithium battery which is mounted on the RTC board. The battery is not rechargeable but it can be easily changed as it is mounted in a socket. The nominal capacity of a standard CR2032 battery is 220mA. At 190nA current draw of the RTC this provides a 1,157,894h (48,245 days / 134 years) of run time. Using the RTC board requires a different OS stack that disables the internal RTC.

The MLUI restores the system time from the RTC on bootup. The RTC is updated automatically from the system time if the system has been updated, either manually or using network time.

When the MLUI is connected to a network and the Internet it sets its system time automatically from a time server using Network Time Protocol (NTP). Once the system time is set the RTCs are synchronized. The RTC time is always kept in UTC time zone. The system time can be set to a local time zone.

System and hardware clocks can be controlled with the Linux command `timedatectl`. Simply invoking

```
root@mlui:~# timedatectl
               Local time: Wed 2022-03-16 20:54:28 UTC
           Universal time: Wed 2022-03-16 20:54:28 UTC
                 RTC time: Wed 2022-03-16 20:54:23
                Time zone: Universal (UTC, +0000)
System clock synchronized: no
              NTP service: active
          RTC in local TZ: no
```

displays the current status of the system clock and the hardware clock. Using

```
root@mlui:~# timedatectl set-timezone "America/Los_Angeles"
```

sets the system timezone (in the example above the timezone is set to Pacific time):

```
root@mlui:~# timedatectl
               Local time: Wed 2022-03-16 14:06:28 PDT
           Universal time: Wed 2022-03-16 21:06:28 UTC
                 RTC time: Wed 2022-03-16 21:06:24
                Time zone: America/Los_Angeles (PDT, -0700)
System clock synchronized: yes
              NTP service: active
          RTC in local TZ: no
```

Please note that changing the timezone only affects the system time. The time of the hardware clock remains in UTC. While the hardware clock can be set to a different timezone it is recommended to keep it at UTC.

For list of timezones use:

```
root@gmlui:~# timedatectl list-timezones
Africa/Abidjan
Africa/Algiers
Africa/Bissau
Africa/Cairo
Africa/Casablanca
...
```

If the MLUI is not connected to a network and cannot obtain the time from a time server, date and time can be set manually:

```
root@mlui:~# timedatectl set-ntp false
root@mlui:~# timedatectl set-time "2022-03-16 11:34:56"
```

The first command disables time NTP synchronization. As long as NTP synchronization is enabled, setting the time manually is diallowed.

All hardware clocks are updated automatically from the system time. However, only one hardware clock is used to set the system time after boot. Hardware clocks can be manipulated directly using the `hwclock` command.

The following reads the default hardware clock that is used to set the system time:

```
root@mlui:~# hwclock -r
2022-03-16 14:14:51.107313-07:00
```

Please note that the output is adjusted for the local time zone. The clock itself maintains the time in UTC.

To update the hardware clock with the current system time:

```
root@mlui:~# hwclock -w
```

To set the system time from the hardware clock:

```
root@mlui:~# hwclock -s
```

The `hwclock` command allows using a specific hardware clock device by specifying the `-f` parameter:

```
root@mlui:~# hwclock -r -f /dev/rtc0
2022-03-16 14:22:43.983579-07:00
```

The `-f` parameter can be used with all `hwclock` subcommands: `-r`, `-w` and `-s`.



## Application Development

To add new applications to the build system two items are required:

* Application Source Code
* Application Build System Recipe

The latter provides instructions to the build system on how to obtain, build, install and package the former. The place for the recipe is inside the build system in the `recipes-application` of the `meta-middleby-disto` layer. The application source code can reside pretty much anywhere and in any form that is supported by the Yocto Project build system. For active application development it is best to use a Git repository on Git server that the build system can access to download the sources from.

Applications can be developed in essentially any programming language including interpreter and bytecode languages such as Python or Java.



## Software Development Kit (SDK)

This section describes how to use the Yocto Project build system to create a software development kit (SDK) only. The document [README-SDK](README-SDK.md) explains the usage of the SDK.

The SDK consists of two parts:

* Development image for the target system
* Development environment for the development host system

You can build both pieces at once with the *build.sh* script:

1. Change to the directory where you have cloned the repository into:
 
   ```
   $ cd ~/middleby/mlui
   ```

2. Invoke the build script:

   ```
   $ tools/build.sh
   ```

The script presents you with an interactive menu:

```
BUILDING DEVICE IMAGE
=====================

1) mlui-lcd7
2) mlui-lcd5
Please choose the machine to build for: 1

1) core-image-mlui
2) core-image-brava
3) core-image-synesso
Please choose the image to build: 2

1) production
2) development
3) sdk
Please choose the image type to build: 3

[...]
```

To build the SDK you have to select *sdk* as the image type. After you have entered the image type the script will first build the target image and then the SDK for the development host system. This might take quite some time in particular if this is the first time you are building an image. After the build has successfully finished the build system places the target imagre into the directory `build/tmp/deploy/images/<machine>` and the SDK installer into the directory `build/tmp/deploy/sdk`.

The SDK installer is a self-contained shell script named `<distribution>-<host system>-<target image>-<target architecture>-toolchain-<version>.sh`:

* `<distribution>` - the name of the distribution e.g. *mlui*
* `<host system>` - the host system type the SDK is built for, always *glibc-X86_64*
* `<target image`> - the name of the target image you have specified for building
* `<target architecture>` - the CPU architecture of the target machine, for the Allwinner A64 it is `cortexa53-crypto`
* `<version>` - SDK version number

For distribution of an SDK you need to provide the following to your developers:

* SDK installer as determined above e.g. mlui-glibc-x86_64-core-image-mlui-dev-cortexa53-crypto-toolchain-0.0.1.sh.
* The document [README-SDK](docs/README-SDK.md) with instructions for SDK usage.



## RAUC Software Updates

The [RAUC Software Update System](https://rauc.readthedocs.io/en/latest/) provides functioality for secure and failsafe updates of embedded devices. RAUC supports different update schemes. For the MLUI the A/B scheme with two identical banks is used. While the system is running from one bank, the other bank can be updated in the background. Once the update is complete a restart of the system switches to the updated bank. If the system starts successfully the bank is marked as the default. A subsequent update will then update the other bank.

RAUC update packages are called *bundles*. A RAUC bundle contains the necessary files, typically partition images, to update the system. To build an update bundle:

1. Change to the directory where you have cloned the repository into:
 
   ```
   $ cd ~/middleby/mlui
   ```

2. Invoke the RAUC bundle build script:

   ```
   $ tools/build-rauc.sh
   ```

The script presents you with an interactive menu:

```
==================================

1) mlui-lcd7
2) mlui-lcd5
3) mlui-lcd7-usmp-t070
Please choose the machine to build for: 3

1) core-image-mlui
2) core-image-viking
3) core-image-synesso
Please choose the image to build: 2

1) production
2) development
Please choose the image type to build: 2

[...]
```

Once the build has completed, the bundle is placed into the `middleby/mlui/build/deploy/image/<machine>` directory. Bundle files have the extension `.raucb`.

Bundles can be installed on the MLUI either from a drive or via HTTP streaming. For the latter, an HTTP server with HTTP streaming support (such as [NGINX()https://www.nginx.com/]) is required.

To update from a drive, copy the bundle to a USB thumb drive and plug into one of the MLUI USB ports. The MLUI mounts the drive automatically on `/media/sda1`. To update the system run:

```
root@mlui:~# rauc install /media/sda1/core-image-mlui-dev-rauc-emmc-<machine>-<timestamp>.raucb

  0% Installing
  0% Determining slot states
 20% Determining slot states done.
 20% Checking bundle
 20% Verifying signature
 40% Verifying signature done.
 40% Checking bundle done.
 40% Checking manifest contents
 60% Checking manifest contents done.
 60% Determining target install group
 80% Determining target install group done.
 80% Updating slots
 80% Checking slot emmc-kernel.1
 85% Checking slot emmc-kernel.1 done.
 85% Copying image to emmc-kernel.1
 90% Copying image to emmc-kernel.1 done.
 90% Checking slot emmc-rootfs.1
 95% Checking slot emmc-rootfs.1 done.
 95% Copying image to emmc-rootfs.1
 99% Copying image to emmc-rootfs.1 done.
 99% Updating slots done.
100% Installing done.
Installing `/media/sda1/core-image-viking-dev-rauc-emmc-mlui-lcd7-20230606172701.raucb` succeeded
```

Updating the root file system partition `emmc-rootfs.1` can take a couple of minutes because of its size. The percentage indicators are not reliable and more of a counter of steps than an indicator for installation time.

It is possible to update a system from development to production and vice-versa. However, it is not possible to update a machine with a RAUC bundle built for a different machine as the machines are not compatible.

After the update has finished, restart the system:

```
root@mlui:~# 
[...]

[ 3307.732621] reboot: Restarting system

U-Boot SPL 2022.01+middlebymlui-gd637294 (Jan 10 2022 - 18:46:34 +0000)
DRAM: 1024 MiB
Trying to boot from MMC2


U-Boot 2022.01+middlebymlui-gd637294 (Jan 10 2022 - 18:46:34 +0000) Allwinner Technology

CPU:   Allwinner A64 (SUN50I)
Model: Middleby MLUI
DRAM:  1 GiB
WDT:   Not starting watchdog@1c20ca0
MMC:   mmc@1c0f000: 0, mmc@1c10000: 2, mmc@1c11000: 1
Loading Environment from FAT... OK
Warning: HDMI PHY init timeout!
Warning: HDMI PHY init timeout!
Warning: HDMI PHY init timeout!
Warning: HDMI PHY init timeout!
In:    serial
Out:   serial
Err:   serial
Net:   No ethernet found.
Hit any key to stop autoboot:  0 
switch to partitions #0, OK
mmc1(part 0) is current device
Scanning mmc 1:1...
Found U-Boot script /boot.scr
1877 bytes read in 1 ms (1.8 MiB/s)
## Executing script at 4fc00000
Found valid RAUC slot B

[...]
```

The message `Found valid RAUC slot B` indicates from what slot the system is booting.

Once the system has booted successfully, RAUC will mark the slot as valid and the system will continue to boot from it.