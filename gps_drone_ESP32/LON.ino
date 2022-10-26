void LON()
{
  DEG = lon / 1000000;
 MIN1 = (lon / 10000) % 100;
 MIN2 = lon % 10000;
 Serial.print("LON :");
 lcd.setCursor(0, 1);
 lcd.print("LON:");
 lcd.print(DEG);
 Serial.print(DEG);
 lcd.write(0xDF);
 lcd.print(MIN1);
 Serial.print(MIN1);
 lcd.print(".");
 lcd.print(MIN2);
 Serial.println(MIN2);
 lcd.print("' ");
 delay(1000);
}
