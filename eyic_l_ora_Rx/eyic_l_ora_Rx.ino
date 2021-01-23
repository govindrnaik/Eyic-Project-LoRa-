#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>


// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//int led = 3;
int val = 0;
int nss = 10;
int RESET = 9;
int dio0 = 2;
String full_string;
String inString = "";
int rssi;
void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  
  LoRa.setPins(nss, RESET, dio0);
  Serial.begin(9600);
 
  //  pinMode(led,OUTPUT);
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);



}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  full_string = "";

  if (packetSize)
  {
    // received a paket
    Serial.println("");
    //Serial.println("...................................");
    Serial.println("Received packet: ");

    // read packet

    while (LoRa.available()) {
      char incoming = (char)LoRa.read();
      full_string += incoming;
      //Serial.print(incoming);
      if (incoming == 'c')
      {
        Serial.print("Error");
      }
      else
      {
        Serial.print(incoming);
        lcd.setCursor(0, 0);
        lcd.print(incoming);
      }
      rssi = LoRa.packetRssi();
     
    }
    Serial.println("");
    Serial.print("RSSI ");
    Serial.print(rssi);
    lcd.setCursor(0, 1);
    lcd.print(rssi);

  }

}
