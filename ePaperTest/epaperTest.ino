#include <SPI.h>
#include "imagedata.h"

#define RST_PIN         8
#define DC_PIN          9
#define CS_PIN          10
#define BUSY_PIN        7
#define WIDTH           640
#define HEIGHT          400

uint32_t image1block, image2block, image3block, far_block4, image4block;

void createFarPointers()
{
  image1block = pgm_get_far_address( image1);
  image2block = pgm_get_far_address( image2);
  image3block = pgm_get_far_address( image3);
  image4block = pgm_get_far_address( image4);
}

void SendCommand(unsigned char command) {
  digitalWrite(DC_PIN, LOW);
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(command);
  digitalWrite(CS_PIN, HIGH);
}

void SendData(unsigned char command) {
  digitalWrite(DC_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(command);
  digitalWrite(CS_PIN, HIGH);

}

void BusyHigh() {
  while (!(digitalRead(BUSY_PIN)));
}

void BusyLow() {
  while ((digitalRead(BUSY_PIN)));
}

void Reset(void) {
  digitalWrite(RST_PIN, HIGH);
  delay(200);
  digitalWrite(RST_PIN, LOW);
  delay(1);
  digitalWrite(RST_PIN, HIGH);
  delay(200);
}

void EPDInit() {
  pinMode(CS_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(DC_PIN, OUTPUT);
  pinMode(BUSY_PIN, INPUT);
  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  Serial.println("Initialized SPI");
  Reset();
  Serial.println("Sent reset commands");
  BusyHigh();
  Serial.println("Busy pin is now high");
  Serial.println("Done reset. Sending init commands to epd");
  SendCommand(0x00);
  SendData(0x2f);
  SendData(0x00);
  Serial.println("Sent 0x00 command");
  SendCommand(0x01);
  SendData(0x37); //trying default of 00001000 orig 0x37
  SendData(0x01); //trying default of 0x01, orig 0x00
  SendData(0x05);
  SendData(0x05);
  Serial.println("Sent 0x01 command");
  SendCommand(0x03);
  SendData(0x00);
  Serial.println("Sent 0x03 command");
  SendCommand(0x06);
  SendData(0xC7);
  SendData(0xC7);
  SendData(0x1D);
  Serial.println("Sent 0x06 command");
  SendCommand(0x41);
  SendData(0x00);
  Serial.println("Sent 0x41 command");
  SendCommand(0x50);
  SendData(0x37);
  Serial.println("Sent 0x50 command");
  SendCommand(0x60);
  SendData(0x22);
  Serial.println("Sent 0x60 command");
  SendCommand(0x61);
  SendData(0x02);
  SendData(0x80);
  SendData(0x01);
  SendData(0x90);
  Serial.println("Sent 0x61 command");
  SendCommand(0xE3);
  SendData(0xAA);
  Serial.println("EPD init commands done. Begin clear");
  SendCommand(0x61);//Set Resolution setting
  SendData(0x02);
  SendData(0x80);
  SendData(0x01);
  SendData(0x90);
  Serial.println("Sent 0x61 command");
  SendCommand(0x10);
  Serial.println("Sent 0x10 command. Writing to screen");

  for (int i = 0; i < WIDTH / 2; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      SendData(0x11);
    }
  }
  Serial.println("Sent all clear commands. Refreshing screen");

  SendCommand(0x04);
  BusyHigh();
  SendCommand(0x12);
  BusyHigh();
  SendCommand(0x02);
  BusyLow();
  delay(500);
}

void setup()
{
  Serial.begin(9600);
  EPDInit();
  Serial.println("Cleared Display");
  SendCommand(0x61);//Set Resolution setting
  SendData(0x02);
  SendData(0x80);
  SendData(0x01);
  SendData(0x90);
  Serial.println("Sent 0x61 command");
  SendCommand(0x10);
  Serial.println("Sent 0x10 command. Writing to screen");
  createFarPointers();
  Serial.println("I finished clearing the screen. Let's print an image");
  SendCommand(0x61);//Set Resolution setting
  SendData(0x02);
  SendData(0x80);
  SendData(0x01);
  SendData(0x90);
  Serial.println("Sent 0x61 command");
  SendCommand(0x10);
  Serial.println("Sent 0x10 command. Writing to screen");

  for (int i = 0; i < 32000; i++) {
    SendData(pgm_read_byte_far(image1block + i));
  }

  for (int i = 0; i < 32000; i++) {
    SendData(pgm_read_byte_far(image2block + i));
  }

  for (int i = 0; i < 32000; i++) {
    SendData(pgm_read_byte_far(image3block + i));
  }

  for (int i = 0; i < 32000; i++) {
    SendData(pgm_read_byte_far(image4block + i));
  }

  Serial.println("Sent all image data. Refreshing screen");
  
  SendCommand(0x04);
  BusyHigh();
  SendCommand(0x12);
  BusyHigh();
  SendCommand(0x02);
  BusyLow();
  delay(500);

  Serial.println("Done");
}

void loop()
{
}
