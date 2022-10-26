void LAT()
{
 Serial.print("LAT :");
 DEG = lat / 1000000;
 MIN1 = (lat / 10000) % 100;
 MIN2 = lat % 10000;
 lcd.setCursor (0,0);
 lcd.print("LAT:");
 lcd.print(DEG);
 Serial.print(DEG);
 lcd.write(0xDF);
 lcd.print(MIN1);
 Serial.print(MIN1);
 lcd.print(".");
 lcd.print(MIN2);
 Serial.println(MIN2);
 lcd.print("' ");
}
