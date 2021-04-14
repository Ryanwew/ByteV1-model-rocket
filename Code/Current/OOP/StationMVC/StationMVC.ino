#include <Wire.h>
class hardwire {
  private:
  byte _rx;

  public:
  void hardwireStart(){
    Wire.begin();
    Wire.onReceive(hardwireReceive);
  }

  static void hardwireReceive(int bytes){
    _rx = Wire.read();
  }

};

hardwire chip;

void setup() {
  chip.hardwireStart();
}

void loop() {

}
