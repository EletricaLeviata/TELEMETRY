/*
 * MODULO DE RECEPÇÃO DE DADOS DO LEVIATÃ
 * O SEGUINTE SISTEMA RECEBERÁ OS DADOS DO BARCO PORAQUÊ,
 * ONDE OS MESMOS SERÃO ANALISADOS DURANTE A COMPETIÇÃO E
 * APÓS A MESMA.
 * 
 * DADOS RELEVANTES:
 * LATITUDE, LONGITUDE, VELOCIDADE.
 * TENSÃO BATERIA, CORRENTE BATERIA, POTENCIA PAINÉL E CORRENTE PAINÉL
 * CORRENTE DO PRIMÁRIO, TENSÃO DO SECUNDÁRIO E TENSÃO DO SECUNDÁRIO.
 */


/*
DIAGRAMA DE MONTAGEM DO RECEPTOR LORA E DISPLAY, JUNTO COM A ESP-32.
  LDC     ESP32       LORA
  GND     GND         -
  VCC     VIN         -
  RS      D15         -
  R/W     D13         -
  E       D14         -
  PSB     GND         -
  RST     GPIO 22     -
  BLA     VDD(3V3)    VCC
  BLK     GND         GND
  -       D2          DIO0
  -       D12         RST
  -       D19         MISO
  -       D5          NSS
  -       D23         MOSI
  PS: TENTAR FAZER A MUDANÇA DO CS, EVITAR CONFLITO DE PORTA
*/

//LORA
#include <LoRa.h>
#include <SPI.h>
#define ss 5    //CHIP SELECTION
#define rst 12  //RESET
#define dio0 2  //ESCOLHIDO PELA APLICAÇÃO
//DISPLAY
#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* CS=*/ 5, /* reset=*/ 22); // DISPLAY PARA ESP32

void setup() {
  u8g2.begin();                           //Iniciando o Display

  Serial.begin(115200);                   //Definindo Velocidade da Serial
  while (!Serial);
  Serial.println("RECEPTOR LORA");        // Serial Iniciou
  LoRa.setPins(ss, rst, dio0);            // Definindo os Pinos do LoRa
  while (!LoRa.begin(433000010))              // 433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");                  // Caso o LoRa não Inicie.
    delay(1000);
  }
  LoRa.setSyncWord(0xA34);                 // não sei oq significa...(testar com 0xF3,0x34,0x12)  //ranges from 0-0xFF, default 0x34, see API docs
  Serial.println("LoRa Initializing OK!");// Mensagem de Início do LoRa.
}

void loop() {

  int packetSize = LoRa.parsePacket();      // Definindo o Pacote como Inteiro.
  if (packetSize) {
    Serial.print("Received packet");        // Mensagem de Pacote Recebido.
    while (LoRa.available())
    {
      String LoRaData = LoRa.readString();  // Fazer a Leitura do Pacote.
      u8g2.clearBuffer();                   // Limpar a memória do Display.
      u8g2.setFont(u8g2_font_ncenB08_tr);   // Escolhendo a Fonte para uso.
      u8g2.setCursor(0, 10);                //Definindo Posição para escrita.
      u8g2.print(LoRaData);                 //Printando no display as informações.
      Serial.print(LoRaData);               //Jogando na Serial os Dados recebidos.
    }
    u8g2.setCursor(0, 20);                  //Definindo posição da Escrita
    u8g2.print(LoRa.packetRssi());
    Serial.print("' with RSSI ");           // Printando o RSSI do Pacote
    Serial.println(LoRa.packetRssi());
    u8g2.sendBuffer();                      // transfer internal memory to the display
  }
  delay(10);//VERIFICAR SE PRECISA...
}
