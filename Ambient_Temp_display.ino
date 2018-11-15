#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <SPI.h>
//#include "SSD1306Ascii.h"
//#include "SSD1306AsciiWire.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

#define LCD_I2C 0x3F
#define SSD_I2C 0x3C
#define DEG_CHAR ((char)223)

LiquidCrystal_PCF8574 lcd(LCD_I2C);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Adafruit_BME280 bme;


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

void setup() {
//  Serial.begin(115200);
//  Serial.println("LCD...");

  Wire.begin();
  Wire.beginTransmission(LCD_I2C);

  lcd.begin(16, 2); // initialize the lcd
  lcd.setBacklight(170);
  lcd.noBlink(); lcd.noCursor(); lcd.clear(); lcd.home();

  p("Checking Sensor...");
  delay(500);
  bool status = bme.begin();
  if (!status) {
    lcd.clear();
    p("Sensor Error!");
  } else {
    lcd.clear();
    p("Sensor Found!");
  }
  delay(1000);

  //  lcd.setCursor(0, 0);
  //  p("Temp    = ");
  //  lcd.setCursor(0, 1);
  //  p("Humidity= ");
  //  lcd.home();
}

void loop() {
  static long time; // = millis();
  if (millis() - time > 1000) {
    printValues();
    time = millis();
  }
}

void p(const char* t) {
  lcd.print(t);
}

void printValues(void) {
  double t = bme.readTemperature(),
         f =  t * 1.8 + 32,
         h = bme.readHumidity();
  // Serial.print(F("Temp="));   Serial.print(t); Serial.print(F("ºC, Humidity=")); Serial.println(h);
  lcd.setCursor(0, 0);
  lcd.print(" "); lcd.print(t); lcd.print(DEG_CHAR); lcd.print("C/");
  lcd.print(f); lcd.print(DEG_CHAR); lcd.print("F");

  lcd.setCursor(0, 1);
  lcd.print(" "); lcd.print(h); p("% Humidity");
}

