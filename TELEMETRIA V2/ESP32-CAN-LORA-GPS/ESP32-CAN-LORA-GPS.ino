
/*
MCP 2515    ESP32       LORA sx1278   GPS
GND         GND         GND           G
VCC         VDD(3V3)    VCC           V
-           D2          DIO0          -
-           D12         RST           -
-           D19         MISO          -
-           D5          NSS           -
SI          D23         MOSI          -
SCK         D18         SCLK          -
SO          D19         -             -
INT         D15         -             -
-           RX0         -             T
-           TX0         -             R

// VOLTÍMETRO (VN,GND)
*/

int i = 0;
int last = millis();

//LORA
#include <LoRa.h>
#include <SPI.h> 
#define ss 22
#define rst 12
#define dio0 2

//CONTADOR
int counter = 0;
long previousMillis = 0;        // Variável de controle do tempo
long redLedInterval = 800;     // Tempo em ms do intervalo a ser executado

//GPS
#include <TinyGPS.h>
#include <Wire.h>
TinyGPS gps;
long lat;
long lon;
unsigned long fix_age;
int DEG;
int MIN1;
int MIN2;
int C;

//CAN
#include <SPI.h>
#include <mcp2515.h>
#define CS 5
struct can_frame canMsg;
MCP2515 mcp2515(CS);
String x; String y; String l; String m; String n; String o; String p; String q;
float a = 0.0; float b = 0.0; float c = 0.0; float d = 0.0; float e = 0.0; float f = 0.0; float g = 0.0; float h = 0.0;

void LAT()
{
 DEG = lat / 1000000;
 MIN1 = (lat / 10000) % 100;
 MIN2 = lat % 10000;
 Serial.print("LAT:");
 Serial.println(lat);
}
void LON()
{
 DEG = lon / 1000000;
 MIN1 = (lon / 10000) % 100;
 MIN2 = lon % 10000;
 Serial.print("LON:");
 Serial.println(lon);
 }

void setup() 
{
//LORA
  Serial.begin(115200); 
  while (!Serial);
  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
  while (!LoRa.begin(433000010))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
  }
  LoRa.setSyncWord(0x34);
  Serial.println("LoRa Initializing OK!");
//CAN
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  Serial.println("------- CAN Read ----------");
}
 
void loop() 
{
 //GPS
 while (Serial.available()){
  digitalWrite (13, HIGH);
  C = Serial.read(); 
  if (gps.encode(C)){}
 }
 digitalWrite (13, LOW);
 gps.get_position(&lat, &lon, &fix_age);
 unsigned long currentMillis = millis();    //Tempo atual em ms
 /* if (currentMillis - previousMillis > redLedInterval) {
    previousMillis = currentMillis;
 LAT(); 
 LON(); 
 }*/
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
   }
   //LORA
  //Serial.print("Sending packet: ");
  LoRa.beginPacket();   //Enviando os pacotes de Dados
  LoRa.print(a/10);
  LoRa.print(b/10);
  LoRa.print(c/10);
  LoRa.print(d/10);
  LoRa.endPacket();
 
  //delay(800);
}
