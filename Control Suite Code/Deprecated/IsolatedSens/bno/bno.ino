#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

bool check = true;

struct datastore {
  unsigned long timer;
  float ox;
  float oy;
  float oz;
  float ax;
  float ay;
  float az;
};

void setup(void)
{
  Serial.begin(115200);

  bno.begin();
  bno.setExtCrystalUse(true);
}

void loop(void)
{
  struct datastore myData;
        
  sensors_event_t orient, accel;
  bno.getEvent(&orient, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&accel, Adafruit_BNO055::VECTOR_LINEARACCEL);

  myData.ox = orient.orientation.x, 4;
  myData.oy = orient.orientation.y, 4;
  myData.oz = orient.orientation.z, 4;

  myData.ax = accel.acceleration.x, 4;
  myData.ay = accel.acceleration.y, 4;
  myData.az = accel.acceleration.z, 4;

  Serial.print(myData.ax); Serial.print(","); Serial.print(myData.ay); Serial.print(","); Serial.println(myData.az);
}
