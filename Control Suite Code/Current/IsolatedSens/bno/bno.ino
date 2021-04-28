#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

bool check = true;

struct datastore {
  unsigned long timer;
  float x;
  float y;
  float z;
  float temp;
  float pres;
  float alti;
};

void setup(void)
{
  Serial.begin(115200);

}

void loop(void)
{
  bno.begin()
        
  sensors_event_t event;
  bno.getEvent(&event);

  myData.x = event.orientation.x, 4;
  myData.y = event.orientation.y, 4;
  myData.z = event.orientation.z, 4;
}
