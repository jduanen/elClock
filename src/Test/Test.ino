/***************************************************************************

 EL Clock Test Program
 
****************************************************************************/

#include "Arduino.h"
#include "PCF8574.h"
#include "DS3232RTC.h"      // https://github.com/JChristensen/DS3232RTC


#define TEST            5
#define VERBOSE         1

#define I2C_BASE_ADDR   0x20
#define READ_ADDR       0x4F
#define WRITE_ADDR      0x4E
#define NUM_UNITS       2    // 4
#define NUM_SEGMENTS    8
#define DIGIT_MAP_SIZE  10

#define DECIMAL_POINT   0b00000010  // decimal point (display pin #3)
#define SEGMENT_A       0b00000001  // bottom horizontal (display pin #2)
#define SEGMENT_B       0b00000100  // right bottom vertical (display pin #7)
#define SEGMENT_C       0b00001000  // left bottom vertical (display pin #8)
#define SEGMENT_D       0b00010000  // middle horizontal (display pin #11)
#define SEGMENT_E       0b00100000  // left top vertical (display pin #14)
#define SEGMENT_F       0b01000000  // right top vertical (display pin #17)
#define SEGMENT_G       0b10000000  // top horizontal (display pin #20)


byte c = 0;

DS3232RTC rtc;

PCF8574 afd[NUM_UNITS] = {
  PCF8574(I2C_BASE_ADDR),
  PCF8574(I2C_BASE_ADDR + 1)
//  PCF8574(I2C_BASE_ADDR + 2),
//  PCF8574(I2C_BASE_ADDR + 3)
};

byte digitMap[DIGIT_MAP_SIZE] = {
  SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_E + SEGMENT_F + SEGMENT_G, // 0
  SEGMENT_B + SEGMENT_F, // 1
  SEGMENT_A + SEGMENT_C + SEGMENT_D + SEGMENT_F + SEGMENT_G, // 2
  SEGMENT_A + SEGMENT_B + SEGMENT_D + SEGMENT_F + SEGMENT_G, // 3
  SEGMENT_B + SEGMENT_D + SEGMENT_E + SEGMENT_F, // 4
  SEGMENT_A + SEGMENT_B + SEGMENT_D + SEGMENT_E + SEGMENT_G, // 5
  SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_D + SEGMENT_E + SEGMENT_G, // 6
  SEGMENT_B + SEGMENT_F + SEGMENT_G, // 7
  SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_D + SEGMENT_E + SEGMENT_F + SEGMENT_G, // 8
  SEGMENT_B + SEGMENT_D + SEGMENT_E + SEGMENT_F + SEGMENT_G // 9
};


void clearUnit(byte unit) {
  byte seg;
  for (seg = 0; seg < NUM_SEGMENTS; seg++) {
    afd[unit].digitalWrite(seg, HIGH);
  }
}


void clearAllUnits() {
  byte unit;
  for (unit = 0; unit < NUM_UNITS; unit++) {
    clearUnit(unit);
  }
}


void condPrint(String s) {
  if (VERBOSE) {
    Serial.print(s);
  }
}


void condPrintln(String s) {
  if (VERBOSE) {
    Serial.println(s);
  }
}


void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(' ');
  Serial.print(day());
  Serial.print(' ');
  Serial.print(month());
  Serial.print(' ');
  Serial.print(year());
  Serial.println();
}


void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(':');
  if(digits < 10) {
      Serial.print('0');
  }
  Serial.print(digits);
}


void setup() { 
  byte unit;

  Serial.begin(9600);
  delay(500);

  for (unit = 0; unit < NUM_UNITS; unit++) {
    afd[unit].pinMode(P0, OUTPUT, HIGH);
    afd[unit].pinMode(P1, OUTPUT, HIGH);
    afd[unit].pinMode(P2, OUTPUT, HIGH);
    afd[unit].pinMode(P3, OUTPUT, HIGH);
    afd[unit].pinMode(P4, OUTPUT, HIGH);
    afd[unit].pinMode(P5, OUTPUT, HIGH);
    afd[unit].pinMode(P6, OUTPUT, HIGH);
    afd[unit].pinMode(P7, OUTPUT, HIGH);
    afd[unit].begin();
  }
  clearAllUnits();

  rtc.begin();
  setSyncProvider(rtc.get);
  if(timeStatus() != timeSet) {
    Serial.println("Unable to sync with the RTC");
  } else {
    Serial.println("RTC has set the system time");
  }

  condPrintln("Start");
}


void enableSegments(byte unit, byte segments) {
  PCF8574::DigitalInput digitalInput;

  digitalInput.p7 = !((segments >> 7) & 0x01);
  digitalInput.p6 = !((segments >> 6) & 0x01);
  digitalInput.p5 = !((segments >> 5) & 0x01);
  digitalInput.p4 = !((segments >> 4) & 0x01);
  digitalInput.p3 = !((segments >> 3) & 0x01);
  digitalInput.p2 = !((segments >> 2) & 0x01);
  digitalInput.p1 = !((segments >> 1) & 0x01);
  digitalInput.p0 = !((segments >> 0) & 0x01);

  afd[unit % NUM_UNITS].digitalWriteAll(digitalInput);
}


void writeDigit(byte unit, byte digit) { 
  byte chr;

  chr = digitMap[digit % DIGIT_MAP_SIZE];
  enableSegments(unit, chr);
}


void loop() {
  byte unit;
  byte seg;

  switch (TEST) {
    case 0:
      // cycle through digits
      for (unit = 0; unit < NUM_UNITS; unit++) {
        condPrint("Unit: " + String(unit) + "; ");
        writeDigit(unit, ((c + unit) % DIGIT_MAP_SIZE));
        condPrintln("Digit: " + String((c + unit) % DIGIT_MAP_SIZE));
      }
      c++;
      condPrintln("-------------");
      break;
    case 1:
      // turn on all segments on all units
      for (unit = 0; unit < NUM_UNITS; unit++) {
        condPrint("Unit: " + String(unit) + "; Segments: ");
        for (seg = 0; seg < NUM_SEGMENTS; seg++) {
          enableSegments(unit, (1 << seg));
          condPrint("0x" + String((1 << seg), HEX) + ", ");
          delay(1000);
        }
        enableSegments(unit, 0);
        condPrintln(";");
      }
      break;
    case 2:
      // cycle through all segments on all units
      for (seg = 0; seg < NUM_SEGMENTS; seg++) {
        condPrint("Segment: " + String(seg) + "; Unit Low: ");
        for (unit = 0; unit < NUM_UNITS; unit++) {
          afd[unit].digitalWrite(seg, LOW);
          condPrint(String(unit) + ", ");
        }
        delay(1000);
        condPrint("; Unit High: ");
        for (unit = 0; unit < NUM_UNITS; unit++) {
          afd[unit].digitalWrite(seg, HIGH);
          condPrint(String(unit) + ", ");
        }
        condPrintln(".");
      }
      break;
    case 3:
      // enable a single segment on each unit
      byte s;
      for (seg = 0; seg < NUM_SEGMENTS; seg++) {
        for (unit = 0; unit < NUM_UNITS; unit++) {
          s = (seg + unit) % NUM_SEGMENTS;
          afd[unit].digitalWrite(s, LOW);
          if (VERBOSE) {
            Serial.println("Unit: " + String(unit) + ", Segment: " + String(s));
          }
        }
        delay(1000);
        clearAllUnits();
        condPrintln("Clear all");
      }
      break;
    case 4:
      // enable all segments, then disable all segments, on unit #0
      for (seg = 0; seg < NUM_SEGMENTS; seg++) {
        afd[0].digitalWrite(seg, LOW);
        delay(1000);
        afd[0].digitalWrite(seg, HIGH);
        delay(1000);
        condPrintln("Segment: " + String(seg));
      }
      break;
    case 5:
      // read and display from RTC module
      int sec, d0, d1;
      sec = second();
      d0 = (sec % 10);
      d1 = ((sec / 10) % 10);
      condPrintln("D1: " + String(d1) + ", D0: " + String(d0));

      if (false) {
        digitalClockDisplay();
      }
      writeDigit(0, d0);
      writeDigit(1, d1);
      break;
    default:
      condPrintln("Invalid Test");
      break;
  }

  delay(1000);
};
