#include <SPI.h>
#include <LoRa.h>

#define DESTINATION_ADDRESS 0xBB
#define LOCAL_ADDRESS 0xFF
#define CMD_RESET 45

byte msgCount = 0;
String outgoingMessage;
String incomingMessage;
unsigned long lastSendTime = 0;
int sendInterval = 2000; // interval in milliseconds

void setup() {
  Serial.begin(9600);
  Serial.println("LoRa Duplex Setup");

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed. Check connections.");
    while (true);
  }

  Serial.println("LoRa initialized successfully.");
}

void loop() {
  if (Serial.available() > 0) {
    incomingMessage += Serial.readString();
    Serial.println("Received via serial: " + incomingMessage);
  }

  if (millis() - lastSendTime > sendInterval && incomingMessage.length() > 0) {
    sendMessage(incomingMessage);
    incomingMessage = ""; // Clear the message after sending
    lastSendTime = millis();
  }

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    onReceive(packetSize);
  }
}

void sendMessage(String message) {
  LoRa.beginPacket();
  LoRa.write(DESTINATION_ADDRESS);
  LoRa.write(LOCAL_ADDRESS);
  LoRa.write(msgCount);
  LoRa.write(message.length());
  LoRa.print(message);
  LoRa.endPacket();

  Serial.println("Sent: " + message + " to " + String(DESTINATION_ADDRESS, HEX));
  msgCount++;
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;

  int recipient = LoRa.read();
  byte sender = LoRa.read();
  byte incomingMsgId = LoRa.read();
  byte incomingLength = LoRa.read();

  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
if (incomingLength != incoming.length()) { return; }  

  Serial.print("Packet received with size: ");
  Serial.println(packetSize);
  Serial.print("Message from: ");
  Serial.println(sender, HEX);
  Serial.print("Message to: ");
  Serial.println(recipient, HEX);
  Serial.print("Message Length: ");
  Serial.println(incomingLength);
  Serial.print("Message Content: ");
  Serial.println(incoming);

  if (recipient != LOCAL_ADDRESS && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;
  }
}
