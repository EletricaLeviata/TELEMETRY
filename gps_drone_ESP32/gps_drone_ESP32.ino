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
int DEG;
int MIN1;
int MIN2;
int c;
//
//************* routine di esposizione della latitudine (in gradi decimali) *************
//
void LAT()
{
 DEG = lat / 1000000;
 MIN1 = (lat / 10000) % 100;
 MIN2 = lat % 10000;
 lcd.setCursor (0,0);
 lcd.print("LAT:");
 lcd.print(DEG);
 lcd.write(0xDF);
 lcd.print(MIN1);
 lcd.print(".");
 lcd.print(MIN2);
 lcd.print("' ");
}
//
// ************ routine di esposizione della longitudine (in gradi decimali) ***********
//
void LON()
{
 DEG = lon / 1000000;
 MIN1 = (lon / 10000) % 100;
 MIN2 = lon % 10000;
 lcd.setCursor(0, 1);
 lcd.print("LON:");
 lcd.print(DEG);
 lcd.write(0xDF);
 lcd.print(MIN1);
 lcd.print(".");
 lcd.print(MIN2);
 lcd.print("' ");
}
//
//
void setup()
{
 Serial.begin(9600); // definisce la velocita' (il baudrate) dei dati dal ricevitore GPS
 lcd.init(); // inizializza il display
lcd.backlight();
 delay(3000); //attende tre secondi per consentire la lettura del display
}
//
//
void loop()
{
 while (Serial.available())
 {
 digitalWrite (13, HIGH);
 c = Serial.read(); // legge i dati provenienti dal modulo GPS
 if (gps.encode(c)) // apparentemente inutile, forse decodifica i dati provenienti dal GPS
 {
 // inserire qui una eventuale codifica di trattamento del segnale ricevuto
 }
 }
 digitalWrite (13, LOW);
 gps.get_position(&lat, &lon, &fix_age); // lat/long (segnati) in millesimi di grado decimale
 LAT(); // lancia la routine di esposizione della latitudine
 LON(); // lancia la routine di esposizione della longitudine
}
