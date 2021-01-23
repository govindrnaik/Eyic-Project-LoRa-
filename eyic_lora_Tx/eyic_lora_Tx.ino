#include <LoRa.h>

int pot = A1;
String  massage;



void setup()
{
  Serial.begin(9600);
  pinMode(pot, INPUT);
  while (!Serial);
  Serial.println("LoRa Transmitter");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setTxPower(20);  //20 max power
  LoRa.setSpreadingFactor(12);  //12 for max range
  LoRa.setSignalBandwidth(62.5E3); //Supported values are 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3=default, and 250E3.

    Serial.println("Please Enter the massage: ");
   while(Serial.available() == 0){
    }
   massage = Serial.readString();
}

void loop()
{
  while(Serial.available()){
     massage = Serial.readString();
    }    
  Serial.println("Sending..... ");

  // send packet
  LoRa.beginPacket();
//  LoRa.print(" Received:");
  LoRa.print(massage);
  LoRa.endPacket();

  Serial.print("Sent Massage:");
  Serial.print(massage);
  Serial.println("");
  delay(50);
}
