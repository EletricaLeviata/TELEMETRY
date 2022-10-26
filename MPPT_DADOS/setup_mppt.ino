void setup_mppt()
{
  Serial.begin(19200);
  Serial1.begin(19200, SERIAL_8N1, 16, 17);                   // Iniciando comunicação do Mppt com o ESP32 através da porta Serial 1
}
