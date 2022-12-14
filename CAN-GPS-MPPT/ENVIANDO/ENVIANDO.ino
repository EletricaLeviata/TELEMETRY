/*MCP2515     ESP32
  INT    -     D15
  SCK    -     D18
  SI     -     D23
  SO     -     D19
  GND    -     GND
  VCC    -     3V3

  CONEXÕES COM MPPT:
  MPPT   -     Arduino pin
  GND    -     GND
  RX     -     -
  TX     -     RX2
  Power        -
*/
////////////////////////////////////////////////////////////////////
//CONFIGURAÇÃO MILLIS
long previousMillis = 0;        // Variável de controle do tempo
long redLedInterval = 900;     // Tempo em ms do intervalo a ser executado
////////////////////////////////////////////////////////////////////
//CONFIGURAÇÃO MPPT
byte len = 5;
int intmeetwaarde;
String label, val;
float meetwaarde;
char buf[45];
char correntebateria[6];
char potenciapainel[6];
char tensaopainel[6];
char tensaobateria[6];
char correntepainel[12];
///////////////////////////////////////////////////////////////////////
//CONFIGURAÇÃO CAN
#include <SPI.h>
#include <mcp2515.h>
#define CS 5
float a = 1.0;    //Tensão DA BATERIA 12V
float b = 93.3;   //Corrente DE SAÍDA
float c = 6.6;    //Constante
//
String Str_correntebateria1;
String Str_tensaobateria1;
String Str_tensaopainel;
String Str_potenciapainel;
//
String myStr_a;
String myStr_b;
String myStr_c;
String teste = "";
//
struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;
struct can_frame canMsg4;
MCP2515 mcp2515(CS);
///////////////////////////////////////////////////////////////////////

void setup() {
  //Tensão
  if (a<10.0 and a >= 1.0) {myStr_a = "0" + String(a);}
  else {myStr_a = String(a);}
  
  //Corrente
  if (b<10.0 and b >= 1.0) {myStr_b = "0" + String(b);}
  else {myStr_b = String(b);}

  //Constante
  if (c<10.0 and c >= 1.0) {myStr_c = "0" + String(c);}
  else {myStr_c = String(c);}

  //Mensagem de Corrente
  canMsg1.can_id  = 36;
  canMsg1.can_dlc = 6;
  canMsg1.data[0] = atof (myStr_a.substring(0, 1).c_str ());
  canMsg1.data[1] = atof (myStr_a.substring(1, 2).c_str ());
  canMsg1.data[2] = atof (myStr_a.substring(3, 4).c_str ());
  //Mensagem de Tensão
  canMsg1.data[3] = atof (myStr_b.substring(0, 1).c_str ());
  canMsg1.data[4] = atof (myStr_b.substring(1, 2).c_str ());
  canMsg1.data[5] = atof (myStr_b.substring(3, 4).c_str ());
  //////////////////////

  //Constante = aplicação de sensor de corrente, rpm
  canMsg2.can_id  = 51;
  canMsg2.can_dlc = 6;
  canMsg2.data[0] = atof (myStr_c.substring(0, 1).c_str ());
  canMsg2.data[1] = atof (myStr_c.substring(1, 2).c_str ());
  //canMsg1.data[2] = atof (myStr_a.substring(2,3).c_str ());
  canMsg2.data[2] = atof (myStr_c.substring(3, 4).c_str ());



  while (!Serial);
  Serial.begin(19200);
  Serial1.begin(19200, SERIAL_8N1, 16, 17);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  Serial.println("Example: Write to CAN");
}

void loop() {
  ///////////

  unsigned long currentMillis = millis();    //Tempo atual em ms

  if (currentMillis - previousMillis > redLedInterval) {
    previousMillis = currentMillis;
    mcp2515.sendMessage(&canMsg1);
    mcp2515.sendMessage(&canMsg2);
    mcp2515.sendMessage(&canMsg3);
    mcp2515.sendMessage(&canMsg4);
  }

  if (Serial1.available())
  {
    loop_mppt();
    Serial.println("");
    //CORRENTE BATERIA-MPPT
    if (atof(correntebateria)<10.0 and atof(correntebateria) >= 1.0) {
      Str_correntebateria1 = "0" + String(atof(correntebateria));
    }
    else {Str_correntebateria1 = String(atof(correntebateria));}
    //TENSAO BATERIA-MPPT
    if (atof(tensaobateria)<10.0 and atof(tensaobateria) >= 1.0) {
      Str_tensaobateria1 = "0" + String(atof(tensaobateria));
    }
    else {Str_tensaobateria1 = String(atof(tensaobateria));}
    //POTENCIA PAINEL-MPPT
    if (atof(potenciapainel)<10.0 and atof(potenciapainel) >= 1.0) {
      Str_potenciapainel = "0" + String(atof(potenciapainel));
    }
    else {Str_potenciapainel = String(atof(potenciapainel));}
    //TENSAO DO PAINEL-MPPT
    if (atof(tensaopainel)<10.0 and atof(tensaopainel) >= 1.0) {
      Str_tensaopainel = "0" + String(atof(tensaopainel));
    }
    else {Str_tensaopainel = String(atof(tensaopainel));}
    /////////////////////////////////////
    canMsg3.can_id  = 12;
    canMsg3.can_dlc = 6;
    canMsg3.data[0] = atof (Str_correntebateria1.substring(0, 1).c_str ());
    canMsg3.data[1] = atof (Str_correntebateria1.substring(1, 2).c_str ());
    canMsg3.data[2] = atof (Str_correntebateria1.substring(3, 4).c_str ());

    canMsg3.data[3] = atof (Str_tensaobateria1.substring(0, 1).c_str ());
    canMsg3.data[4] = atof (Str_tensaobateria1.substring(1, 2).c_str ());
    canMsg3.data[5] = atof (Str_tensaobateria1.substring(3, 4).c_str ());
    ///
    canMsg4.can_id  = 20;
    canMsg4.can_dlc = 6;
    canMsg4.data[0] = atof (Str_potenciapainel.substring(0, 1).c_str ());
    canMsg4.data[1] = atof (Str_potenciapainel.substring(1, 2).c_str ());
    canMsg4.data[2] = atof (Str_potenciapainel.substring(3, 4).c_str ());

    canMsg4.data[3] = atof (Str_tensaopainel.substring(0, 1).c_str ());
    canMsg4.data[4] = atof (Str_tensaopainel.substring(1, 2).c_str ());
    canMsg4.data[5] = atof (Str_tensaopainel.substring(3, 4).c_str ());
  }
}
