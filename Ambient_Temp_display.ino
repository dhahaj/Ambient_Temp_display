#include <BME280I2C.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <SPI.h>

#define SERIAL_BAUD 9600

#ifdef SERIAL_BAUD
#include <PrintEx.h>
StreamEx s = Serial;
using namespace ios;
#endif

#define LCD_I2C 0x3F
#define SSD_I2C 0x3C
#define BME_I2C 0x76
#define DEG_CHAR ((char)223)

LiquidCrystal_PCF8574 lcd(LCD_I2C);  // set the LCD address to 0x27 for a 16 chars and 2 line display

BME280::TempUnit unit = BME280::TempUnit_Fahrenheit;

// Optimized for humidity sensing
BME280I2C::Settings settings(
  BME280::OSR_X1,
  BME280::OSR_X1,
  0, /*BME280::OSR_X1,*/
  BME280::Mode_Forced,
  BME280::StandbyTime_1000ms,
  BME280::Filter_Off,
  BME280::SpiEnable_False,
  BME_I2C // I2C address. I2C specific.
);

BME280I2C bme(settings);

void setup() {
#ifdef SERIAL_BAUD
  Serial.begin(115200);
  s << F("LCD...") << endl;
#endif

  lcd.begin(16, 2); // initialize the lcd
  lcd.setBacklight(150);
  lcd.noBlink(); lcd.noCursor(); lcd.clear(); lcd.home();

  Wire.begin();
  while (!bme.begin()) {
#ifdef SERIAL_BAUD
    s << F("Could not find BME280I2C sensor!") << endl;
#endif
    lcd.home(); lcd.clear();
    lcd.print("No Sensor Found!");
    delay(1500);
  }

  switch (bme.chipModel()) {
    case BME280::ChipModel_BME280:
#ifdef SERIAL_BAUD
      s << F("Found BME280 sensor! Success.") << endl;
#endif
      lcd.home(); lcd.clear();
      lcd.print("Found BME280!");
      break;
    case BME280::ChipModel_BMP280:
#ifdef SERIAL_BAUD
      s << F("Found BMP280 sensor! No Humidity available.") << endl;
#endif
      lcd.home(); lcd.clear();
      lcd.print("Found BMP280!");
      break;
    default:
#ifdef SERIAL_BAUD
      s << F("Found UNKNOWN sensor! Error!") << endl;
#endif
      lcd.home(); lcd.clear();
      lcd.print("Found UNKNOWN sensor!");
  }

  bme.setSettings(settings);
  delay(1000);

}

void loop() {
  printBME280Data(&Serial);
  delay(500);
  /*
    static long time; // = millis();
    if (millis() - time > 1000) {
    printValues();
    time = millis();
    }*/
}

void p(const char* t) {
  lcd.print(t);
}

void printBME280Data(Stream* client) {
  float temp(NAN), hum(NAN), pres(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Fahrenheit);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, unit, presUnit);
#ifdef SERIAL_BAUD
  client->print("Temp: ");
  client->print(temp);
  client->print("°" + String(tempUnit == BME280::TempUnit_Celsius ? 'C' : 'F'));
  client->print("\t\tHumidity: ");
  client->print(hum);
  client->print("% RH");
#endif

  lcd.setCursor(0, 0);
  lcd.print(bme.temp(BME280::TempUnit_Fahrenheit));
  lcd.print(DEG_CHAR);
  lcd.print("F");
  lcd.setCursor(8, 0);
  lcd.print(bme.temp(BME280::TempUnit_Celsius));
  lcd.print(DEG_CHAR);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print(bme.hum());
  lcd.print("%RH ");


  delay(500);
}

/*
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
  }*/
