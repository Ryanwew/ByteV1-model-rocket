#include <SD.h>
#include <SPI.h>

float timer;

File xorientation;

struct datastore {
    uint16_t i1;
};

void setup() {
  Serial.begin(115200);
  SD.begin(10);
}

void loop() {
  struct datastore myData;
  myData.i1 ++;
  timer = millis();
  xorientation = SD.open("x.dat", FILE_WRITE);
  xorientation.write((const uint8_t *)&myData, sizeof(myData));
  xorientation.close();
}
