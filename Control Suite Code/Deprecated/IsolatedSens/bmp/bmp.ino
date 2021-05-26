#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

struct datastore {
  float temp;
  float pres;
  float alti; 
};

void setup() {

  Serial.begin(115200);

  bmp.begin();

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
    Adafruit_BMP280::SAMPLING_X1,     /* Temp. oversampling 2*/
    Adafruit_BMP280::SAMPLING_X1,    /* Pressure oversampling 16*/
    Adafruit_BMP280::FILTER_X2,      /* Filtering. 16*/
    Adafruit_BMP280::STANDBY_MS_125); /* Standby time. */

}

void loop() {
  struct datastore myData;
  
  myData.temp = bmp.readTemperature();

  myData.pres = bmp.readPressure();

  myData.alti = bmp.readAltitude(1013.25);

  Serial.println(myData.alti);

}
