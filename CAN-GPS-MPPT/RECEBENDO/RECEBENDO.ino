// DISPLAY -> ARDUINO MEGA
//GND      -> GND
//VCC      -> 5V
//SDA      -> SDA 21
//SCL      -> SCL 22

/*MCP2515     ESP32
 *INT         D15 
 *SCK         D18 
 *SI          D23 
 *SO          D19
 *GND         GND
 *VCC         3V3

 *GPS(PIN)  -    ESP32(PIN)      //    LCD     -     ESP32(PIN)
 *V         -    3V3             //    GND     -     GND
 *T         -    RX0             //    VCC     -     5V
 *R         -    TX0             //    SDA     -     GPIO 21
 *G         -    GND             //    SCL     -     GPIO 22
*/
#include <SPI.h>
#include <mcp2515.h>
#define CS 5
///////////////////////////////////////////////////////
//display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,20,4);
////////////////////////////////////////////////////////
struct can_frame canMsg;


MCP2515 mcp2515(CS);

String x;
String y;
String l;
String m;
String n;
String o;
String p;
String q;

float a = 0.0;
float b = 0.0;
float c = 0.0;
float d = 0.0;
float e = 0.0;
float f = 0.0;
float g = 0.0;
float h = 0.0;

void setup() {
  lcd.init();
  Serial.begin(9600);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
}

void loop() {


   if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    //Serial.print(canMsg.can_id); // print ID
    if (canMsg.can_id  == 36){
      
     
      Serial.print("PRIMEIRA MSG: ");

      x = String((canMsg.data[0]))+String((canMsg.data[1]))+String((canMsg.data[2]));
      a = x.toFloat();
      Serial.println(a/10);

      y = String((canMsg.data[3]))+String((canMsg.data[4]))+String((canMsg.data[5]));
      b = y.toFloat();
      Serial.println(b/10);

    }
    if (canMsg.can_id  == 51){
      
      Serial.print("SEGUNDA MSG: ");
      /*Serial.print(canMsg.data[0]);
      Serial.print(canMsg.data[1]);
      Serial.print(canMsg.data[2]);
      Serial.print(canMsg.data[3]);
      Serial.print(canMsg.data[4]);
      Serial.println(canMsg.data[5]);
*/
      l = String((canMsg.data[0]))+String((canMsg.data[1]))+String((canMsg.data[2]));
      c = l.toFloat();
      Serial.println(c/10);
/*
      m = String((canMsg.data[3]))+String((canMsg.data[4]))+String((canMsg.data[5]));
      d = m.toFloat();
      Serial.println(d/10);
      */
    }
    if (canMsg.can_id  == 12){
      
      Serial.print("CORRENTE BATERIA: ");

      n = String((canMsg.data[0]))+String((canMsg.data[1]))+String((canMsg.data[2]));
      e = n.toFloat();
      Serial.print(e/10);
      Serial.println(" A");
      //////////////
      Serial.print("TENSAO BATERIA: ");
      o = String((canMsg.data[3]))+String((canMsg.data[4]))+String((canMsg.data[5]));
      f = o.toFloat();
      Serial.print(f/10);
      Serial.print(" V");
      
    }
    if (canMsg.can_id  == 20){    
      Serial.print("POTENCIA DO PAINEL: ");
      p = String((canMsg.data[0]))+String((canMsg.data[1]))+String((canMsg.data[2]));
      g = p.toFloat();
      Serial.print(g/10);
      Serial.println(" W");
      //////////////
      Serial.print("TENSAO PAINEL: ");
      q = String((canMsg.data[3]))+String((canMsg.data[4]))+String((canMsg.data[5]));
      h = q.toFloat();
      Serial.print(h/10);
      Serial.print(" V");     
    }

//////////////////////////////
//DISPLAY PRINT
/////////////////////////////
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("B.CORRENTE: ");
  lcd.print(e/10);
  lcd.setCursor(0,1);
  lcd.print("B.TENSAO: ");
  lcd.print(f/10);
  lcd.setCursor(0,2);
  lcd.print("P.POTENCIA: ");
  lcd.print(g/10);
  lcd.setCursor(0,3);
  lcd.print("P.TENSAO: ");
  lcd.print(h/10);
    
  }    
  Serial.println(" ");
  delay(10);
  }
