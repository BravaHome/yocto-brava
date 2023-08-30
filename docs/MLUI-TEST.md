![ibeeto](artwork/ibeeto-logo.png "ibeeto")



# MLUI Hardware Test

Test procedures and result for MLUI hardware.


## Test Procedures

Index | Test                                   | Description                     | Target
----: | -------------------------------------- | ------------------------------- | ----------------------------------------
1.1   | Local Power Entry J4, Fuse F1          | F1 Pin 2 Voltage                | Voltage applied to J4: 12V-24V +/-10%
1.2   | Local Power Entry LTC4359 Ideal Diode  | R40 Voltage                     | Voltage applied to J4: 12V-24V +/-10%
1.3   | SSR_IO Power Entry J2, Fuse F2         | F2 Pin 2 Voltage                | Voltage applied to J2: 12V-24V +/-10%
1.4   | SSR_IO Power Entry LTC4359 Ideal Diode | R231 Voltage                    | Voltage applied to J2: 12V-24V +/-10%
1.5   | Main 5V Regulator, LED D11             | LED D11 light up                | LED D11 light up green
1.6   | PMIC Power Good LED D3                 | LED D3 light up                 | LED D3 light up green
1.7   | Reset Button                           | Reset Button functional         | Push-and-hold: D3 off, release: D3 on
1.8   | AP-RESET TP24                          | Voltage on TP24                 | 3.0V +/-10%
1.9   | VCC-RTC TP30                           | Voltage on TP30                 | 3.0V +/-10%
1.10  | VCC-PL TP11                            | Voltage on TP11                 | 3.3V +/-10%
1.11  | USB-VBUS TP29                          | Voltage on TP23                 | 5.0V +/-10%
1.12  | DC_HV TP18                             | Voltage on TP18                 | Supply voltage: 12V-24V +/-10%
1.13  | VCC-PG TP9                             | Voltage on TP9                  | 3.3V +/-10%
1.14  | VCC-SENSOR TP51                        | Voltage on TP51                 | 3.3V +/-10%
1.15  | VCC-DRAM TP26                          | Voltage on TP26                 | 1.5V +/-10%
1.16  | PS TP17                                | Voltage on TP17                 | 5.0V +/-10%
1.17  | GPIO1-LDO TP13                         | Voltage on TP13                 | 0V off (3.3V +/- 10% on)
1.18  | VDD-CPUX TP31                          | Voltage on TP31                 | 1.1V +/-10%
1.19  | VDD-SYS TP22                           | Voltage on TP22                 | 1.1V +/-10%
1.20  | DOVDD-CSI TP21                         | Voltage on TP21                 | 0V off (2.8V +/-10% on)
1.21  | ELDO2 TP45                             | Voltage on TP45                 | 0V off (1.9V +/-10% on)
1.22  | DVDD-CSI TP42                          | Voltage on TP42                 | 0V off (1.8V +/-10% on)
1.23  | VCCIV2-HSIC TP39                       | Voltage on TP39                 | 0V off (1.2V +/-10% on)
1.24  | VDD-CPUS TP23                          | Voltage on TP23                 | 1.1V +/-10%
1.25  | AVDD-CSI TP16                          | Voltage on TP16                 | 2.8V +/-10%
1.26  | CPVDD TP28                             | Voltage on TP28                 | 1.8V +/-10%
1.27  | AVCC TP12                              | Voltage on TP12                 | 3.0V +/-10%
1.28  | DLDO4 TP20                             | Voltage on TP20                 | 3.3V +/-10%
1.29  | VCC-5V TP7                             | Voltage on TP7                  | 5.0V +/-10%
1.30  | VCC-CTP TP15                           | Voltage on TP15                 | 3.3V +/-10%
1.31  | VCC-MIPI TP14                          | Voltage on TP14                 | 3.3V +/-10%
1.32  | DCDC1 TP10                             | Voltage on TP10                 | 3.3V +/-10%
2.1   | Bootloader                             | u-boot console output           | Output:<br>CPU: Alwinner A64 (SUN50I)<br>Model: Middleby MLUI<br>DRAM: 1024 MiB
2.2   | Linux Boot                             | Linux login prompt              | Output: mlui-<display> login:
2.3   | Display                                | Display shows boot splash       | Boot splash screen on display (Middleby, Synesso)
2.4   | Touch Screen                           | Touch screen responding         | Output from evtest for single, multi and gestures
2.5   | USB-A J1                               | USB flash drive recognized      | Output: scsi host0: usb-storage 4-1.1:1.0
2.6   | USB-A J100                             | USB flash drive recognized      | Output: scsi host0: usb-storage 4-1.2:1.0
2.7   | USB-C J6                               | USB flash drive recognized      | Output: scsi host0: usb-storage 1-1:1.0
2.8   | Piezo Buzzer                           | Voltage on J7 Pin 2             | 5.0V +/-10%
2.9   | Audio Amplifier Stereo                 | Audio playback on J8            | Stereo audio file playback
3.1   | WiFi List                              | List WiFi SSIDs                 | nmcli d wifi, SSIDs available
3.2   | WiFi Connect                           | Connect to WiFi network         | nmcli d wifi connect <SSID> password <secret>
4.1   | Hardware Clock Set                     | Set hardware clock              | hwclock -w
4.2   | Hardware Clock Read                    | Read hardware clock             | hwclock -r
4.3   | Hardware Clock Restore                 | Set system time from hw clock   | hwclock -s
4.4   | Hardware Clock Battery                 | System time maintained          | System time restored from hardware clock with battery
5.1   | I2C Buses                              | Detect I2C buses (2)            | Output: i2cdetect -l
5.2   | I2C Devices on Bus 0                   | I2C Devices on Bus 0            | 0x20 - NXP PCA9574 GPIO extender <br> ST Microelectronics STLM75 temperature sensor <br> 0x68 - MicroCrystal RV-4162 RTC
5.3   | I2C Devices on Bus 1                   | I2C Devices on Bus 1            | 0x4a - Atmel Maxtouch touch screen controller (only with Orient Displays)
6.1   | Display glmark2                        | glmark2-es2-drm score           | glmark2 score: 40 +/- 10%


## MLUI Rev4 Test Results

Index | Result                                                                   | Status
----: | ------------------------------------------------------------------------ | ---------
1.1   | 11.97V                                                                   | OK
1.2   | 11.94V                                                                   | OK
1.3   | 11.7V                                                                    | OK
1.4   | 11.69V                                                                   | OK
1.5   | LED D11 green                                                            | OK
1.6   | LED D3 green                                                             | OK
1.7   | LED D3 off and back on                                                   | OK
1.8   | 3.14V                                                                    | OK
1.9   | 3.13V                                                                    | OK
1.10  | 3.32V                                                                    | OK
1.11  | 5.03V                                                                    | OK
1.12  | 11.94V                                                                   | OK
1.13  | 3.28V                                                                    | OK
1.14  | 3.32V                                                                    | OK
1.15  | 1.39V                                                                    | OK
1.16  | 4.95V                                                                    | OK
1.17  | 0.22V                                                                    | OK
1.18  | 1.08V                                                                    | OK
1.19  | 1.13V                                                                    | OK
1.20  | 27mV                                                                     | OK
1.21  | 27mV                                                                     | OK
1.22  | 27mV                                                                     | OK
1.23  | 27mV                                                                     | OK
1.24  | 1.12V                                                                    | OK
1.25  | 2.82V                                                                    | OK
1.26  | 1.83V                                                                    | OK
1.27  | 3.02V                                                                    | OK
1.28  | 3.32V                                                                    | OK
1.29  | 5.03V                                                                    | OK
1.30  | 3.31V                                                                    | OK
1.31  | 3.32V                                                                    | OK
1.32  | 3.31V                                                                    | OK
2.1   | CPU: Alwinner A64 (SUN50I)<br>Model: Middleby MLUI<br>DRAM: 1024 MiB     | OK
2.2   | mlui-lcd7 login:                                                         | OK
2.3   | boot splash                                                              | OK
2.4   | evtest reporting touch                                                   | OK
2.5   | scsi host0: usb-storage 4-1.1:1.0                                        | OK
2.6   | scsi host0: usb-storage 4-1.2:1.0                                        | OK
2.7   | csi host0: usb-storage 1-1:1.0                                           | OK
2.8   | 5.05V                                                                    | OK
2.9   | Stereo file playing                                                      | OK
3.1   | WiFi network list                                                        | OK
3.2   | WiFi connect to network                                                  | OK
4.1   | HW clock set                                                             | OK
4.2   | HW clock read                                                            | OK
4.3   | HW clock set system time                                                 | Ok
4.4   | HW clock restore after boot                                              | OK
5.1   | i2c-0, i2c-1                                                             | OK
5.2   | Devices: 0x20, 0x48, 0x68                                                | OK
5.3   | Devices: 0x4a                                                            | OK
6.1   | glmark2 score: 40                                                        | OK



