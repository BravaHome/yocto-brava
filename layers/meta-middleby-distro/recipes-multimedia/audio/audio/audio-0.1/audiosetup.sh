#!/bin/sh

/usr/bin/amixer set 'Headphone' off
/usr/bin/amixer set 'Line Out' off
/usr/bin/amixer set 'DAC' on
/usr/bin/amixer set 'AIF1 Slot 0 Digital DAC' on
/usr/bin/amixer set 'AIF1 DA0 Stereo' 'Stereo'
/usr/bin/amixer set 'Line Out Source' 'Mono Differential'
/usr/bin/amixer set 'Line Out' 50%
/usr/bin/amixer set 'Line Out' on
