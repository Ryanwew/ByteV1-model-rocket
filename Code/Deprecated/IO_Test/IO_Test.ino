#include <Wire.h>

void setup() {
  pinMode(8, OUTPUT);

}

void loop() {
  digitalWrite(8, HIGH);
  delay(3000);
  digitalWrite(8, LOW);
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(9); 
    Wire.endTransmission();
  delay(1000);
}
