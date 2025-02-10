#include<SPI.h>

unsigned int b = 16;
void sendValue(unsigned long int value)
{
  static bool firstSend = true;
  uint8_t hByte;
  uint8_t m2Byte;
  uint8_t m1Byte;
  uint8_t lByte;

  hByte = value >> 24;
  m2Byte = value >> 16;
  m1Byte = value >> 8;
  lByte = value;
    
    SPI.beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0));
    
    SPI.transfer(hByte);
    SPI.transfer(m2Byte);
    SPI.transfer(m1Byte);
    SPI.transfer(lByte);
    
    SPI.endTransaction();

  delayMicroseconds(500);
}
void setup() {
  SPI.begin();
  delay(200);
  sendValue(70002);  // Change this to change frequency, 70000 - 16Mhz, 70001 - 8MHz, 70002 - 4MHz
  sendValue(255);    // Change this to set increment value (1 - 65535)
  delay(1000);
sendValue(70001);  // Change this to change frequency, 70000 - 16Mhz, 70001 - 8MHz, 70002 - 4MHz
delay(1000);
  sendValue(255);    // Change this to set increment value (1 - 65535)


}

void loop() {

  // put your main code here, to run repeatedly:

}
