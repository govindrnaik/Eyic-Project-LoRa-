#include <LoRa.h>

String  smassage;
String  incoming;
int packetSize;
int nss = 10;
int RESET = 9;
int dio0 = 2;

String outgoing;              // outgoing message
byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

void setup() {
  LoRa.setPins(nss, RESET, dio0);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Welcome");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setTxPower(20);  //20 max power
  LoRa.setSpreadingFactor(12);  //12 for max range
  LoRa.setSignalBandwidth(62.5E3); //Supported values are 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3=default, and 250E3.

  Serial.println("you can start chatting");


}

void loop() {
  packetSize = LoRa.parsePacket();
  //  for receiver
  receivee(packetSize);
  //for Transmitter
  transmit();
}

void transmit() {
  if (Serial.available()) {
    String smassage = Serial.readString();
    // send packet
    LoRa.beginPacket();                   // start packet
    LoRa.write(destination);              // add destination address
    LoRa.write(localAddress);             // add sender address
    LoRa.write(msgCount);                 // add message ID
    LoRa.write(smassage.length());        // add payload length
    LoRa.print(smassage);                 // add payload
    LoRa.endPacket();                     // finish packet and send it
    msgCount++;                           // increment message ID

    Serial.println("");
    Serial.print("<< ");
    Serial.print(smassage);
  }
}

void receivee(int packetSize) {
  if (packetSize)
  {
    // received a paket
    Serial.println("");
    Serial.print(">> ");

    // read packet header bytes:
    int recipient = LoRa.read();          // recipient address
    byte sender = LoRa.read();            // sender address
    byte incomingMsgId = LoRa.read();     // incoming msg ID
    byte incomingLength = LoRa.read();    // incoming msg length
    String incoming = "";                 // payload of packet

    // read packet
    while (LoRa.available()) {
      incoming += (char)LoRa.read();      // add bytes one by one
    }
    Ack(incoming);
    if (incoming == "@@@@") {   // check length for error
      Serial.println("Received from: 0x" + String(sender, HEX));
      Serial.println("Sent to: 0x" + String(recipient, HEX));
      Serial.println("Message ID: " + String(incomingMsgId));
      Serial.println("Acknowledged");
      Serial.println("RSSI: " + String(LoRa.packetRssi()));
      Serial.println("Snr: " + String(LoRa.packetSnr()));
      Serial.println();
      return;                             // skip rest of function
    }

    Serial.println("Received from: 0x" + String(sender, HEX));
    Serial.println("Sent to: 0x" + String(recipient, HEX));
    Serial.println("Message ID: " + String(incomingMsgId));
    Serial.println("Message length: " + String(incomingLength));
    Serial.println("Message: " + incoming);
    Serial.println("RSSI: " + String(LoRa.packetRssi()));
    Serial.println("Snr: " + String(LoRa.packetSnr()));
    Serial.println();
  }
}

void Ack(String ) {
  if (incoming != "@@@@") {
    LoRa.beginPacket();                   // start packet
    LoRa.write(destination);              // add destination address
    LoRa.write(localAddress);             // add sender address
    LoRa.write(msgCount - 1);               // add message ID
    LoRa.write("00");        // add payload length
    LoRa.print("@@@@");                 // add payload
    LoRa.endPacket();
  }
}
