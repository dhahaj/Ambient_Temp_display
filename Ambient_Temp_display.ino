#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define I2C_ADDR 0x3F
LiquidCrystal_PCF8574 lcd(I2C_ADDR);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  Serial.println("LCD...");

  Wire.begin();
  Wire.beginTransmission(I2C_ADDR);

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
  delay(5000);

  lcd.setCursor(0, 0);
  p("Temp=");
  lcd.setCursor(0, 1);
  p("Humidity=");
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
  // lcd.home(); //lcd.clear();
  //p("Temp=");
  lcd.setCursor(6, 0);
  lcd.print(bme.readTemperature());
  lcd.setCursor(10, 1);
  // p("Humidity=");
  lcd.print(bme.readHumidity());
  p('%');
}

