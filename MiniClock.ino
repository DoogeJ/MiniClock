#include <Arduino.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <SPI.h>
#include <Wire.h>
#include <ezTime.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/* This code is guaranteed nasty.
 * Written by Jaap-Willem Dooge / DoogeJ / NoxiousPluK
 * https://github.com/DoogeJ/MiniClock
 * August 1, 2020
 * 
 * Released under Zlib license:
 * https://github.com/DoogeJ/MiniClock/blob/master/licence.txt
 */

const char *ssid = "Your SSID";
const char *password = "Your Password";

bool colon = true;
String timeString;
String dateString;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

Timezone Netherlands;
int count = 1;

void setup()
{
  WiFi.begin(ssid, password);
  waitForSync();
  Netherlands.setLocation("Europe/Amsterdam");
  Wire.pins(0, 2); // set GPIO 0 as SDA, GPIO 2 as SCL
  Wire.begin();
  u8g2.begin();
}

void loop()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_open_iconic_all_4x_t);
  if (count < 6)
  {
    //handle time
    u8g2.drawStr(0, 32, "{"); // clock icon
    u8g2.setFont(u8g2_font_inr16_mn);
    if (colon)
    {
      timeString = Netherlands.dateTime("H:i");
      colon = false;
    }
    else
    {
      timeString = Netherlands.dateTime("H i");
      colon = true;
    }
    char __timeString[sizeof(timeString)];
    timeString.toCharArray(__timeString, sizeof(__timeString));
    u8g2.drawStr(48, 24, __timeString);
  }
  else
  {
    //handle date
    u8g2.drawStr(0, 32, "k"); // calendar icon
    u8g2.setFont(u8g2_font_inr16_mn);
    dateString = Netherlands.dateTime("d-m");
    char __dateString[sizeof(dateString)];
    dateString.toCharArray(__dateString, sizeof(__dateString));
    u8g2.drawStr(48, 24, __dateString);
    if (count > 9)
    {
      count = 0;
    }
  }
  u8g2.sendBuffer();

  delay(1000);
  events(); //this makes sure that ezTime occasionally updates the internal timer
  count++;
}
