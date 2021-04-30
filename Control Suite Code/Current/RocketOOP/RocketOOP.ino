#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>

class hardwire {
  private:
  byte _txid;
  byte _tx;
  byte _rx;

  public:
  void hardwireStart(byte txid){
    _txid = txid;
    Wire.begin();
  }

  hardwireQuery(byte val){
    _tx = val;
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(_tx); 
    Wire.endTransmission();
    Wire.begin(9);  
  }

  hardwireRx(){
    return(_rx);
  }

  hardwireSet(byte bytes){
    _rx = bytes;
  }
};

hardwire pixel;

struct datastore {
  unsigned long timer;
  float ox;
  float oy;
  float oz;
  float ax;
  float ay;
  float az;
  float alt;
  float temp
  byte state;
};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
