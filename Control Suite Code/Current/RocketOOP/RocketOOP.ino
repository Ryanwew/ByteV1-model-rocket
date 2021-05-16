#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

File dataFile;
byte transferSuccess = false;
byte rx;

struct datastore {
  unsigned long timer;
  float ox;
  float oy;
  float oz;
  float ax;
  float ay;
  float az;
  float alti;
  float temp;
  byte state;
  byte errReg;
};

struct datastore myData;

class hardwire {
  private:
  byte _txid;
  byte _tx;
  byte _rx;

  public:
  void hardwireStart(byte txid){
    _txid = txid;
    Wire.begin(txid);
  }

  hardwireQuery(byte val){
    _tx = val;
    Wire.begin(); //check if this is needed
    Wire.beginTransmission(9); // transmit to device #8
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

class sensor {
  private:
  bool hasChecked;
  float _ASL;

  public:

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

  sensorStart(float ASL){
    _ASL = ASL;
    
    if(!hasChecked){
      if (bno.begin() && SD.begin(10) && bmp.begin()){
        bno.setExtCrystalUse(true);
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
        Adafruit_BMP280::SAMPLING_X1,     /* Temp. oversampling 2*/
        Adafruit_BMP280::SAMPLING_X1,    /* Pressure oversampling 16*/
        Adafruit_BMP280::FILTER_X2,      /* Filtering. 16*/
        Adafruit_BMP280::STANDBY_MS_1); /* Standby time. 125*/
        hasChecked = true;
        return(true);
      }

      else{
        myData.errReg ++;
        return(false);
      }
    }
  }
  
};

hardwire pixel;
sensor devices;

void setup() {
  Wire.onReceive(hardwireReceive);
  Serial.begin(9600);
  myData.state = 1;
  pixel.hardwireStart(8);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  devices.sensorStart(1100);
}

void hardwireReceive(int bytes){
  rx = Wire.read();
  pixel.hardwireSet(rx);
}

void loop() {
  myData.timer = millis();
  devices.sensorLoop();
  
  /*
  switch (myData.state) {
    case 1:

      if(pixel.hardwireRx() == 2){
        pixel.hardwireQuery(2);
      }

      if(pixel.hardwireRx() == 3){
        transferSuccess = devices.sensorStart(1100);
        if(transferSuccess){
          pixel.hardwireQuery(3);
        }
        else{
          pixel.hardwireQuery(4);
        }
      }

      //exit conditions
      if(transferSuccess){
        transferSuccess = false;
        myData.state = 2;
      }

      beep(myData.timer, 1000);
      break;
      
    case 2:
      devices.sensorLoop();

      if(pixel.hardwireRx() == 5){
        if(myData.errReg == 0){
          pixel.hardwireQuery(5);
          transferSuccess = true;
        }
        else{
          pixel.hardwireQuery(6);
        }
      }


      //exit conditions
      if(transferSuccess){
        myData.state = 3;
        transferSuccess = false;
      }

      beep(myData.timer, 300);
      break;

    case 3:
      devices.sensorLoop();

      beep(myData.timer, 800);
      break;
      
    default:
      myData.errReg ++;
      beep(myData.timer, 100);
      break;
  }
 */
}

void beep(unsigned long delayTime, int pause){
  static bool state;
  static unsigned long lastTime;
  if((delayTime - lastTime) > pause){
    if(state){
      //digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      state = false;
      lastTime = delayTime;
    }
    else{
      //digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      state = true;
      lastTime = delayTime;      
    }
  }
  
}
