# EL Clock
Display made with four 7-Segment electroluminescent display units

Works with [Alien Font Display driver board](https://github.com/jduanen/alienFontDisplay)

Running in timer mode
![EL Timer](eltimer.gif)

*TBD*

## IEL-O-VI C63.396.208-01
* Green, 7-segment electroluminescent display with dot on the bottom right
* same number of segments and same pinout as the -02 version
* pinout
  - 1: common
  - 2: bottom horizontal (M)
  - 3: bottom right dot (XS)
  - 5: n/c
  - 7: right bottom vertical (M)
  - 8: left bottom vertical (M)
  - 11: middle horizontal (M)
  - 14: left top vertical (M)
  - 17: right top vertical (M)
  - 20: top horizontal (M)
* per-segment capacitance (estimates)
  - XS (1x): ? pFd
  - M  (8x): ?? pFd

=======================================================================================

* Driver (bit number) to Segment (display pin #) map
  - 0: bottom horizontal (2)
  - 1: decimal point (3)
  - 2: right bottom vertical (7)
  - 3: left bottom vertical (8)
  - 4: middle horizontal (11)
  - 5: left top vertical (14)
  - 6: right top vertical (17)
  - 7: top horizontal (20)

* DS3231 AT24C32 Real Time Clock Module (Parallax)
  - 32K EEPROM
  - 3.3-5.5V
  - I2C @ 400KHz

* TBD
  - instrument using SensorNet library
  - use SensorNet to subscribe to time set broadcast
    * create a cron job that publishes time periodically
      - send multiple samples in a burst
    * update RTC if delta exists between it and majority of samples
  - consider muxing display units to extend lifetime of units
  - build SWMP to drive the segments at 225VAC @ 400Hz
    * 4mA per segment, max of 28 segments on: <120mA @ 225VAC (<30W)

* Notes
  - run driver on 5V, but can use 3V3 GPIOs from ESP12F to drive the I2C bus to all the port exapanders and the RTC

=======================================================================================

* Links
  - http://www.155la3.ru/electroluminescent.htm
  - https://www.industrialalchemy.org/articleview.php?item=1123
  - http://lampes-et-tubes.info/cd/cd113.php?l=f
  - https://groups.google.com/g/neonixie-l
  - https://www.nixology.uk/el-displays/
  - https://www.acmesystems.it/ESP12
  - https://www.parallax.com/product/ds3231-at24c32-real-time-clock-module/
  - https://www.parallax.com/package/ds3231-datasheet/
  - https://github.com/JChristensen/DS3232RTC

* notes
  - HV PSU:
    * Adafruit 448 12V EL Wire/Tape Inverter 
    * https://www.amazon.com/gp/product/B07MMMNCQL
    * https://www.amazon.com/gp/product/B007R9UBQ0/
