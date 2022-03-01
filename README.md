# elClock
Display made with four 7-Segment electroluminescent display units

Works with [Alien Font Display driver board](https://github.com/jduanen/alienFontDisplay)

*TBD*

* Driver (bit number) to Segment (display pin #) map
  - 0: bottom horizontal (2)
  - 1: decimal point (3)
  - 2: right bottom vertical (7)
  - 3: left bottom vertical (8)
  - 4: middle horizontal (11)
  - 5: left top vertical (14)
  - 6: right top vertical (17)
  - 7: top horizontal (20)

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

* Links
  - http://www.155la3.ru/electroluminescent.htm
  - https://www.industrialalchemy.org/articleview.php?item=1123
  - http://lampes-et-tubes.info/cd/cd113.php?l=f
  - https://groups.google.com/g/neonixie-l
  - https://www.nixology.uk/el-displays/
  - https://www.acmesystems.it/ESP12


* notes
  - HV PSU:
    * Adafruit 448 12V EL Wire/Tape Inverter 
    * https://www.amazon.com/gp/product/B07MMMNCQL
    * https://www.amazon.com/gp/product/B007R9UBQ0/
