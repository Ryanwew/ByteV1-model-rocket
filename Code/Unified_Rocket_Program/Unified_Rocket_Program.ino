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

  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  buttonid = Wire.read();    // read one character from the I2C
  
}

void loop(void)
{
  Serial.println(check);
  if(go == false){  //buttonid == 4 && 
    check = false;
    Wire.begin();
    go = true;
  }
  
  if(check == false){
      if (bno.begin() && SD.begin(10) && bmp.begin()){
        check = true;
        digitalWrite(6, HIGH);
        Serial.print("BNO Good");
        bno.setExtCrystalUse(true);

        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                      Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                      Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                      Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                      Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

        //delay(200);
        
        Wire.begin();
      
        Wire.beginTransmission(9); // transmit to device #9
        Wire.write(icstate);              // sends x 
        Wire.endTransmission();    // stop transmitting
    }    
  }
 else if(go && check){
  timer = millis();
  
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  x = event.orientation.x, 4;
  y = event.orientation.y, 4;
  z = event.orientation.z, 4;

  xx=x;
  
  if(xx> 359){
    xx=xx-360;
  }

  temp = bmp.readTemperature();
  //Serial.print(" C: ");
  //Serial.print(temp);
  tempbarometer = SD.open("temp.txt", FILE_WRITE);
  tempbarometer.print(temp); tempbarometer.print(" ");
  tempbarometer.println(timer);
  tempbarometer.close();

  pres = bmp.readPressure();
  //Serial.print(" Pa: ");
  //Serial.print(pres);
  presbarometer = SD.open("pres.txt", FILE_WRITE);
  presbarometer.print(pres); presbarometer.print(" ");
  presbarometer.println(timer);
  presbarometer.close();

  alti = bmp.readAltitude(1013.25);
  //Serial.print(" M: ");
  //Serial.print(alti);
  altibarometer = SD.open("alti.txt", FILE_WRITE);
  altibarometer.print(pres); altibarometer.print(" ");
  altibarometer.println(timer);
  altibarometer.close();

  //Serial.print(" Xtru: ");
  //Serial.print(xx);
  xorientation = SD.open("x.txt", FILE_WRITE);
  xorientation.print(xx); xorientation.print(" ");
  xorientation.println(timer);
  xorientation.close();

  //Serial.print(" Y: ");
  //Serial.print(y);
  yorientation = SD.open("y.txt", FILE_WRITE);
  yorientation.print(y); yorientation.print(" "); 
  yorientation.println(timer);
  yorientation.close();

  //Serial.print(" Z: ");
  //Serial.println(z);
  zorientation = SD.open("z.txt", FILE_WRITE);
  zorientation.print(z); zorientation.print(" ");
  zorientation.print(timer);
  zorientation.close();
  
  delay(BNO055_SAMPLERATE_DELAY_MS);
 }
}
