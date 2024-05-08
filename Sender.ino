#include <SPI.h>              // include libraries
#include <LoRa.h>
 
byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xFF;     // address of this device
byte destination = 0xBB;      // destination to send to
 
 
String outgoing;              // outgoing message
String Mymessage;             // stores message from the bluetooth
 
int calling;                  // stores the command
 
void setup() {
  Serial.begin(9600);                   // initialize serial
  Serial.println("LoRa Duplex");
 
  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
 
  Serial.println("LoRa init succeeded.");
}
 
void loop() {
 
   if(Serial.available()>0)
   {
    //read data
    Mymessage=Serial.readString();
    sendMessage(Mymessage);
    delay(100);
    Mymessage = "";
   }
 
 
      // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}
 
void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}
 
void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
 
  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
 
  String incoming = "";
 
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
 
  if (incomingLength != incoming.length()) {   // check length for error
   // Serial.println("error: message length does not match length");
   ;
    return;                             // skip rest of function
  }
 
  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    //Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
 
  // if message is for this device, or broadcast, print details:
 // Serial.println("Received from: 0x" + String(sender, HEX));
 // Serial.println("Sent to: 0x" + String(recipient, HEX));
  //Serial.println("Message ID: " + String(incomingMsgId));
 // Serial.println("Message length: " + String(incomingLength));
  //Serial.println("Message: " + incoming);
  //Serial.println("RSSI: " + String(LoRa.packetRssi()));
  //Serial.println("Snr: " + String(LoRa.packetSnr()));
  //Serial.println();
    String q = getValue(incoming, ',', 0);
    calling = q.toInt();
    //Serial.println(q);
if(calling== 45)
{
 calling = 0;  
    
  }
 
 else 
  Serial.println(q);
//incoming = "";
  
}
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}