#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>

File dataFile;

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
  float temp;
  byte state;
  byte errReg;
};

void setup() {
  Wire.onReceive(hardwireReceive);
  Serial.begin(115200);
}


void hardwireReceive(int bytes){
  rx = Wire.read();
  pixel.hardwireSet(rx);
}

void loop() {
  struct datastore myData;

}

void sensorLoop() {

  myData.temp = bmp.readTemperature();

  myData.alti = bmp.readAltitude(1013.25);

  sensors_event_t orient, accel;
  bno.getEvent(&orient, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&accel, Adafruit_BNO055::VECTOR_LINEARACCEL);

  myData.ox = orient.orientation.x, 4;
  myData.oy = orient.orientation.y, 4;
  myData.oz = orient.orientation.z, 4;

  myData.ax = accel.acceleration.x, 4;
  myData.ay = accel.acceleration.y, 4;
  myData.az = accel.acceleration.z, 4;

  dataFile = SD.open("log.dat", FILE_WRITE);
  dataFile.write((const uint8_t *)&myData, sizeof(myData));
  dataFile.close();
}
