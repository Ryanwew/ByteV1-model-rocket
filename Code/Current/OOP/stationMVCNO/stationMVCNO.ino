byte _rx;
#include <Wire.h>

void setup() {
  Wire.begin();
  Wire.onReceive(hardwireReceive);

}

void hardwireReceive(int bytes){
  _rx = Wire.read();
}

void loop() {
  // put your main code here, to run repeatedly:

}
