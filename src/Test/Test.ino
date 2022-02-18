/***************************************************************************

 EL Clock Test Program
 
****************************************************************************/

#include "Arduino.h"
#include "PCF8574.h"

// TODO invert binary values in tables and remove negation in code

#define TEST            1
#define VERBOSE         1

#define I2C_BASE_ADDR   0x20
#define READ_ADDR       0x4F
#define WRITE_ADDR      0x4E
#define NUM_UNITS       4
#define NUM_SEGMENTS    8
#define DIGIT_MAP_SIZE  10


unsigned char c = 0;

PCF8574 afd[NUM_UNITS] = {
  PCF8574(I2C_BASE_ADDR),
  PCF8574(I2C_BASE_ADDR + 1),
  PCF8574(I2C_BASE_ADDR + 2),
  PCF8574(I2C_BASE_ADDR + 3)
};

unsigned char digitMap[DIGIT_MAP_SIZE] = {
  0b00000001 + 0b00000100 + 0b00100000 + 0b01000000, // 0
  0b00100000, // 1
  0b00000001 + 0b00001000 + 0b00010000 + 0b01000000, // 2
  0b00000001 + 0b00000010 + 0b00010000 + 0b01000000, // 3
  0b00000100 + 0b00010000 + 0b10000000, // 4
  0b00000001 + 0b00000010 + 0b01000000 + 0b10000000, // 5
  0b00000001 + 0b00000010 + 0b00001000 + 0b01000000 + 0b10000000, // 6
  0b00000100 + 0b01000000, // 7
  0b00000001 + 0b00000010 + 0b00001000 + 0b00010000 + 0b01000000 + 0b10000000, // 8
  0b00000100 + 0b00010000 + 0b01000000 + 0b10000000 // 9
};


void clearUnit(unsigned char unit) {
  unsigned char seg;
  for (seg = 0; seg < NUM_SEGMENTS; seg++) {
    afd[unit].digitalWrite(seg, HIGH);
  }
}


void clearAllUnits() {
  unsigned char unit;
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


void setup() { 
  unsigned char unit;

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

  condPrintln("Start");
}


void writeDigit(unsigned char unit, unsigned char digit) { 
  unsigned char chr;
  PCF8574::DigitalInput digitalInput;

  chr = digitMap[digit % DIGIT_MAP_SIZE];
  digitalInput.p7 = !((chr >> 7) & 0x01);
  digitalInput.p6 = !((chr >> 6) & 0x01);
  digitalInput.p5 = !((chr >> 5) & 0x01);
  digitalInput.p4 = !((chr >> 4) & 0x01);
  digitalInput.p3 = !((chr >> 3) & 0x01);
  digitalInput.p2 = !((chr >> 2) & 0x01);
  digitalInput.p1 = !((chr >> 1) & 0x01);
  digitalInput.p0 = !((chr >> 0) & 0x01);

  afd[unit % NUM_UNITS].digitalWriteAll(digitalInput);
  condPrint("writeDigit: 0x" +  String(chr, HEX) + "; ");
}


void loop() {
  unsigned char unit;
  unsigned char seg;

  switch (TEST) {
    case 0:
      // cycle through digits
      for (unit = 0; unit < NUM_UNITS; unit++) {
        condPrint("Unit: " + String(unit) + "; ");
        writeDigit(unit, ((c + unit) % DIGIT_MAP_SIZE));
        condPrintln("Digit: " + String((c + unit) % DIGIT_MAP_SIZE));
      }
      c++;
      println("-------------");
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
      unsigned char s;
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
    default:
      condPrintln("Invalid Test");
      break;
  }

  delay(1000);
};
