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
Use thin guage wire of the appropriate length.

![Mohawk GPS assembly](assembly.jpg)

## Firmware

Mohawk firmware runs on small ESP32 dev boards, using the arduino framework. These are super cheap but powerful microcontrollers. PlatformIO is used to manage the software.

### Program the device

Install Visual Studio Code and PlatformIO.
Open the project in the `arduino` directory of this project.
Connect the ESP32 to the computer by USB.
Use the PlatformIO "Upload" function to program the device.
