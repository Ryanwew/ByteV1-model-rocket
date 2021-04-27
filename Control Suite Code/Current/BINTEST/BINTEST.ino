#include <SD.h>
#include <SPI.h>

float timer;

File xorientation;

struct datastore {
  unsigned long timing;
  float data;
  byte state;
};

void setup() {
  Serial.begin(115200);
  SD.begin(10);
}

void loop() {
  struct datastore myData;
  myData.data += 1;
  myData.timing = millis();
  myData.state = 3;
  xorientation = SD.open("x2.dat", FILE_WRITE);
  xorientation.write((const uint8_t *)&myData, sizeof(myData));
  xorientation.close();
}
