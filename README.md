# BASEline Mohawk GPS

[![workflow status](https://github.com/platypii/Mohawk/actions/workflows/ci.yml/badge.svg)](https://github.com/platypii/Mohawk/actions)
[![mit license](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Mohawk is a helmet mounted wireless GPS for wingsuit, skydive, and BASE jumping.
The key advantages are:

 - Wireless data streaming to BASEline app
 - Aerodynamic shape conforms to most helmets

Mohawk is open source, designed for makers.
Using cheap components, soldering, and a 3D printer, you can make your own Mohawk GPS.

![Mohawk GPS on a skydiving helmet](helmet.jpg)

## Components

 - TinyPICO ESP32 microcontroller $30
 - Beitian BN-180 GPS $20
 - 3.7V 200mAh LiPo battery 502025 $8
 - Slide switch
 - LED status light
 - 3D printed case
 - M3 screw

## Wiring

```
    +-------+
    |23  BAT|-------+
    |19  GND|       |
    |18   5V|       |
    |5    3V|-----+ |
    |22    4|     | o
LED-|21   14|     |  \
+---|32   15|     | o
| +-|33   27|     | |
| | |RST  26|     | |
| | |GND  25|     | |
| | +-------+     | |
| |               | |
| +-------------+ | |
+-------------+ | | |
              | | | |
    +-------+ | | | |
    |GPS    |-+ | | |
    |       |---+ | |
    |      +|-----+ |
    |      -|--o    |
    +-------+       |
                    |
    +-------+       |
    |Batt   |       |
    |      -|--o    |
    |      +|-------+
    |       |
    +-------+
```

## Assembly

Connect the components according to the above schematic.
Use thin-guage wire of the appropriate length.

![Mohawk GPS assembly](assembly.jpg)

## Firmware

Mohawk firmware runs on small ESP32 dev boards, using the arduino framework.
These are cheap but powerful microcontrollers. PlatformIO is used to manage the software.

### Program the device

Install Visual Studio Code and PlatformIO.
In VSCode, open the `arduino` directory of this repository.
Connect the ESP32 to the computer by USB.
Use the PlatformIO "Upload" function to program the device.

## Mohawk Protocol

Mohawk GPS sends location data via Bluetooth Low Energy (LE).
The protocol is a custom protocol designed to compress information to as little data as possible.
The goal is to minimize the time broadcasting on the bluetooth radio.
This helps reduce interfere with GPS, and saves battery power.

Mohawk location message fields:

| field | size | description |
| --- | --- | --- |
| msg_type | 1 byte | 'L' |
| time | 3 bytes | three least significant bytes of tenths since epoch |
| latitude | 4 byte int | microdegrees |
| longitude | 4 byte int | microdegrees |
| altitude | 2 byte short | decimeters above -100m MSL |
| vN | 2 byte short | velocity north cm/s |
| vE | 2 byte short | velocity east cm/s |
| climb | 2 byte short | climbrate cm/s |

The 2 byte fields `altitude`, `vN`, `vE`, and `climb` represent `NaN` as maximum short value `0x7fff`.
