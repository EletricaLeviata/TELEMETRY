// GPS(PIN)  -    ESP32(PIN)      //    LCD     -     ESP32(PIN)
// V         -    3V3             //    GND     -     GND
// T         -    RX0             //    VCC     -     5V
// R         -    TX0             //    SDA     -     GPIO 21
// G         -    GND             //    SCL     -     GPIO 22
//
#include <TinyGPS.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

TinyGPS gps;
long lat;
long lon;
unsigned long fix_age;
int DEG,MIN1,MIN2,c;
//int MIN1;
//int MIN2;
//int c;

void setup()
{
lcd.init();
lcd.backlight();
Serial.begin(9600);
pinMode (13, OUTPUT);
delay(3000); 
}
//
//
void loop()
{
 while (Serial.available())
 {
 digitalWrite (13, HIGH);
 c = Serial.read();
 }
 digitalWrite (13, LOW);
 gps.get_position(&lat, &lon, &fix_age);
 LAT();
 LON();
}
