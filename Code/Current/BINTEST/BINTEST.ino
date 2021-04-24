#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_BMP280.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

bool check = true;

int icstate = 1;

float x = 0;
float y = 0;
float z = 0;

float xx = 0;

float temp;
float pres;
float alti;

float timer;

File xorientation;
File yorientation;
File zorientation;

File tempbarometer;
File presbarometer;
File altibarometer;

int buttonid = 0;

bool go = false;

Adafruit_BMP280 bmp;

void setup(void)
{
  Serial.begin(115200);

  Wire.begin(8);

  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  buttonid = Wire.read();    // read one character from the I2C
  
}

void loop(void)
{

  if(check == false){
  SD.begin(10)
  pres = 10;
  //Serial.print(" Pa: ");
  //Serial.print(pres);
  presbarometer = SD.open("pres.txt", FILE_WRITE);
  presbarometer.print(pres);
  presbarometer.close();

}
