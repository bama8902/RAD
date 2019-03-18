/********************************************************
  Clock setting function for Adafruit M0 feather board.  This will set the RTC to the current computer date/time
  and the RTC will maintain that time as long as a battery is connected.  If disconnected just flash this again to
  reset it.

  Written by Lex Kravitz
  August, 2018

  For use with 2 bottle sipper device:
  https://hackaday.io/project/160388-automated-mouse-homecage-two-bottle-choice-test-v2

  Or PIR logger device:


  This project code includes code from:
  *** Adafruit, who made the hardware breakout boards and associated code ***

  Cavemoa's excellent examples of datalogging with the Adalogger:
  https://github.com/cavemoa/Feather-M0-Adalogger

  Uses Arduino Time library http://playground.arduino.cc/code/time
  Maintained by Paul Stoffregen https://github.com/PaulStoffregen/Time

  Uses Arduino Zero RTC library https://www.arduino.cc/en/Reference/RTC
  Maintained by Arturo Guadalupi https://github.com/arduino-libraries/RTCZero

  This project is released under the terms of the Creative Commons - Attribution - ShareAlike 3.0 license:
  human readable: https://creativecommons.org/licenses/by-sa/3.0/
  legal wording: https://creativecommons.org/licenses/by-sa/3.0/legalcode
  Copyright (c) 2018 Lex Kravitz

********************************************************/

/********************************************************
  Setup code
********************************************************/
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <RTCZero.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TimeLib.h> //include the Arduino Time library
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// initialize variables (these will be overwritten so doesn't really matter what they are)
byte months = 1;
byte days = 1;
byte years = 1;
byte hours = 1;
byte minutes = 1;
byte seconds = 1;

char s_month[5];
int tmonth, tday, tyear, thour, tminute, tsecond;
static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

RTCZero rtc;

void setup() {
  /********************************************************
       These commands set the time to the current computer time - unfortunately the device needs to be reflashed to update the time
     ********************************************************/
  rtc.begin(); // initialize RTC

  // __DATE__ is a C++ preprocessor string with the current date in it.
  // It will look something like 'Mar  13  2016'.
  // So we need to pull those values out and convert the month string to a number.
  sscanf(__DATE__, "%s %d %d", s_month, &tday, &tyear);

  // Similarly, __TIME__ will look something like '09:34:17' so get those numbers.
  sscanf(__TIME__, "%d:%d:%d", &thour, &tminute, &tsecond);

  // Find the position of this month's string inside month_names, do a little
  // pointer subtraction arithmetic to get the offset, and divide the
  // result by 3 since the month names are 3 chars long.
  tmonth = (strstr(month_names, s_month) - month_names) / 3;

  months = tmonth + 1;  // The RTC library expects months to be 1 - 12.
  days = tday;
  years = tyear - 2000; // The RTC library expects years to be from 2000.
  hours = thour;
  minutes = tminute;
  seconds = tsecond;

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(days, months, years);


  /********************************************************
       Start, clear, and setup the display
     ********************************************************/
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0); // Where c is a value from 0 to 255 (sets contrast e.g. brightness)

  display.clearDisplay();
  display.display();
  display.setRotation(4);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

/********************************************************
  Main loop
********************************************************/
void loop() {
  display.setCursor(5, 0);
  display.print ("RTC has been set to:");

  display.fillRoundRect (00, 10, 132, 12, 1, BLACK);

  display.setCursor(5, 12);
  display.print(rtc.getMonth());
  display.print("/");
  display.print(rtc.getDay());
  display.print("/");
  display.print(rtc.getYear() + 2000);
  display.print(" ");
  display.print(rtc.getHours());
  display.print(":");
  if (rtc.getMinutes() < 10)
    display.print('0');      // Trick to add leading zero for formatting
  display.print(rtc.getMinutes());
  display.print(":");
  if (rtc.getSeconds() < 10)
    display.print('0');      // Trick to add leading zero for formatting
  display.print(rtc.getSeconds());

  display.setCursor(5, 25);
  display.print ("Reflash device code");
  //display.clearDisplay();
  display.display();

  delay (1000);
}
