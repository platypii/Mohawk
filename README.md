# BASEline Mohawk GPS

Mohawk is a helmet mounted wireless GPS for wingsuit, skydive, and BASE jumping.
The key advantages are:

 - Wireless data streaming to BASEline app
 - Aerodynamic shape conforms to most helmets

Mohawk is open source, designed for makers.
Using cheap components, soldering, and a 3d printer, you can make your own Mohawk GPS.

## Components

 - TinyPICO ESP32 microcontroller $30
 - Beitian BN-180 GPS $16
 - 3.7V 200mAh LiPo battery 502025 $8
 - 3D printed case

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
