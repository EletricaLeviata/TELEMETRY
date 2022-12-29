/* 
// MCP2515     ESP32
 +*INT         D15 
 *SCK         D18 
 *SI          D23 
 *SO          D19
 *GND         GND
 *VCC         3V3

  GPS(PIN)  -    ESP32(PIN)      //    LCD     -     ESP32(PIN)
 *V         -    3V3             //    GND     -     GND
 *T         -    RX0             //    VCC     -     5V
 *R         -    TX0             //    SDA     -     GPIO 21
 *G         -    GND             //    SCL     -     GPIO 22
 *
// VOLTÍMETRO (VN,GND)
*/
//CONFIGURAÇÃO MILLIS
long previousMillis = 0;        // Variável de controle do tempo
long redLedInterval = 900;     // Tempo em ms do intervalo a ser executado


#include <SPI.h>
#include <mcp2515.h>
#define CS 5
///////////////////////////////////////////////////////
//display
#include <TinyGPS.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,20,4);
////////////////////////////////////////////////////////


TinyGPS gps;
long lat;
long lon;
unsigned long fix_age;
int DEG;
int MIN1;
int MIN2;
int C;

/////////////////////////////////////////////////////////

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

//////////////////////////////////////////

void LAT()
{
 DEG = lat / 1000000;
 MIN1 = (lat / 10000) % 100;
 MIN2 = lat % 10000;
 lcd.setCursor (0,0);
 
 Serial.print("LAT:");
 Serial.print(DEG);
 Serial.print(0xDF);
 Serial.print(MIN1);
 Serial.print(".");
 Serial.print(MIN2);
 Serial.println("' ");
 
 lcd.print("LAT:");
 lcd.print(DEG);
 lcd.write(0xDF);
 lcd.print(MIN1);
 lcd.print(".");
 lcd.print(MIN2);
 lcd.print("' ");
}
///////////////
//BATERIA

byte i = 0;                                  //  Variável para usar no for
float accuml = 0.0;            //  Variável para guardar o valor acumulado da temperatura
int analogInput = 36;
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0; // resistência de R1 (100K)
float R2 = 10000.0; // resistência de R2 (10K)
int value = 0;
/////////////


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

 
 Serial.print("LON:");
 Serial.print(DEG);
 Serial.print(0xDF);
 Serial.print(MIN1);
 Serial.print(".");
 Serial.print(MIN2);
 Serial.print("' ");
 }


//////////////////////////////////////////

void setup() {
  pinMode(analogInput, INPUT);
  
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");

}


void loop() {
/*
  //////////////////
  //BATERIA SECUNDARIO
  value = analogRead(analogInput);
  vout = (value *3.7) / 4095.0; // see text
  vin = vout / (R2 / (R1 + R2));
  if (vin < 0.3) {
    vin = 0.0; // declaração para anular a leitura indesejada!
  }

  for ( i = 0; i < 10; i++)
  {
    vin = vout / (R2 / (R1 + R2)) ;
    //Serial.println(vin);
    accuml = accuml + vin;
    //delay(10);
  }
  vin = accuml / 10;
  Serial.println(vin);
  accuml = 0;
*/
  /////////////////////////////////////////////////////////////
  //GPS
 while (Serial.available()){
  digitalWrite (13, HIGH);
  C = Serial.read(); 
  if (gps.encode(C)){}
 }
 digitalWrite (13, LOW);
 gps.get_position(&lat, &lon, &fix_age);
 unsigned long currentMillis = millis();    //Tempo atual em ms

  if (currentMillis - previousMillis > redLedInterval) {
    previousMillis = currentMillis;
 //LAT(); 
 //LON(); 
 }
//////////////////////////////////////////////////////////////////////////////////////////////////////
//CAN
   if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
   
    if (canMsg.can_id  == 36){
      Serial.print("CORRENTE DA BATERIA MPPT: ");
      x = String((canMsg.data[0]))+String((canMsg.data[1]))+String((canMsg.data[2]));
      a = x.toFloat();
      Serial.println(a/10);
      Serial.print("TENSÃO DA BATERIA MPPT: ");
      y = String((canMsg.data[3]))+String((canMsg.data[4]))+String((canMsg.data[5]));
      b = y.toFloat();
      Serial.println(b/10);
    }
    
    if (canMsg.can_id  == 51){
      Serial.print("POTENCIA DO PAINEL MPPT: ");
      l = String((canMsg.data[0]))+String((canMsg.data[1]))+String((canMsg.data[2]));
      c = l.toFloat();
      Serial.println(c/10);
      Serial.print("TENSÃO PAINEL MPPT: ");
      m = String((canMsg.data[3]))+String((canMsg.data[4]))+String((canMsg.data[5]));
      d = m.toFloat();
      Serial.println(d/10);
      Serial.println("-----------------------------------");
    }
    /*
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
*/
  }    
  //delay(100);
  }
