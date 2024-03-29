// Custom two-way communication with Bluetooth HM-10
// Transmitting & receiving data in the form of a struct
//
// Writes and reads (hardcoded) data to the software UART 
//  and prints the transmitted & received packet


// Library to make a Software UART
#include <SoftwareSerial.h>

#include <Wire.h>

#define RX 3
#define TX 2

int acount;
int atru;

int x;

int counte;

SoftwareSerial BTSerial(RX, TX); // (RX, TX)

// Struct to hold the data we want to transmit/receive
struct Packet {
  byte a;
  byte b;
} pkt_tx, pkt_rx;

#define BAUDRATE 9600
 
void setup() {
  // Start Serial Monitor for feedback
  Serial.begin(BAUDRATE);

  // HM-10 default speed in AT command mode
  BTSerial.begin(BAUDRATE);

  pinMode(8, OUTPUT);
  pinMode(9, INPUT);

  Wire.begin();
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C
  
}

void loop() {  
  if(pkt_rx.a == 1){
    acount ++;
    if (acount >3){
      atru = 1;
    }
  }
  else {
    atru = 0;
    acount = 0;
  }

  
  /*
  counte ++;
  Serial.print(counte);
  if (counte > 200){
    Serial.print("counte");
    counte = 0;
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(9); 
    Wire.endTransmission();
    Serial.print("finished");
    if (x == 9){
      pkt_tx.b = 1;
      digitalWrite(8, LOW);
    }
    else{
      pkt_tx.b = 0;
      digitalWrite(8, HIGH);
    }
  }
  */
  
  if(x == 5){
    pkt_tx.a = 1;
  }
  else{
    pkt_tx.a = 0;
  }

  

  if(atru == 1){
    digitalWrite(8, HIGH);
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(3); 
    Wire.endTransmission();
  }
  else if (atru == 0){
    digitalWrite(8, LOW);
  }
  // Receive data from the bluetooth
  bluetooth_receive();

  // Necessary forced delay, if we transmit too fast (no delay)
  //  the error rate increases
  delay(50);
}

// Function responsible for transmitting data over bluetooth
void bluetooth_transmit() {
  // Update data to be transmitted


  // Write packet data to the bluetooth - and transmit
  BTSerial.write((byte *) & pkt_tx,sizeof(Packet));

  // Print the Packet contents
  Serial.print("TX: (a)=(");
  Serial.print(pkt_tx.a); Serial.print(",");
  Serial.println(")");  
}

// Function responsible for receiving data over bluetooth
void bluetooth_receive() {
  // Counting variable to fix a lost connection
  static byte count = 10;
  
  // Check the software serial buffer for data to read
  if(BTSerial.available() >= sizeof(Packet)) {
    // Read in the appropriate number of bytes to fit our Packet
    BTSerial.readBytes((byte *) & pkt_rx,sizeof(Packet));

    // Print the Packet contents
    Serial.print("RX: (a)=(");
    Serial.print(pkt_rx.a); Serial.print(",");
    Serial.println(")");

    // Flush the serial buffer
    while(BTSerial.available() > 0)
      BTSerial.read();   
    
    // Transmit data via bluetooth
    bluetooth_transmit();
  } else {
    // If a disconnect happens, start transmitting
    if(count >= 10) {
      count=0;  // Reset counter
      // Transmit to revive process
      bluetooth_transmit();
    }
    count++;
  }
}
