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

struct datastore {
  unsigned long timer;
  float xx;
  float y;
  float z;
  float temp;
  float pres;
  float alti;
};

float x = 0;

int buttonid = 0;

bool go = false;

Adafruit_BMP280 bmp;

File login;

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
  struct datastore myData;
  /*
  if (buttonid == 9) {
    Wire.begin();
      
    Wire.write(9);       // sends x 
    Wire.endTransmission();    // stop transmitting 
    Wire.begin();
  }
  */
  Serial.println(check);
  if(buttonid == 3 && go == false){  //buttonid == 4 && 
    check = false;
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
        Wire.write(5);       // sends x 
        Wire.endTransmission();    // stop transmitting

        digitalWrite(5, HIGH);
        delay(300);
        digitalWrite(5, LOW);
        delay(50);
        digitalWrite(5, HIGH);
        delay(100);
        digitalWrite(5, LOW);
        delay(50);
        digitalWrite(5, HIGH);
        delay(100);
        digitalWrite(5, LOW);
    }    
     else{
        Wire.beginTransmission(9); // transmit to device #9
        Wire.write(5);       // sends x 
        Wire.endTransmission();    // stop transmitting    
     }
  }
 else if(go && check){
  myData.timer = millis();
  
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  x = event.orientation.x, 4;
  myData.y = event.orientation.y, 4;
  myData.z = event.orientation.z, 4;

  myData.xx=x;
  
  if(myData.xx> 359){
    myData.xx=myData.xx-360;
  }

  myData.temp = bmp.readTemperature();

  myData.pres = bmp.readPressure();

  myData.alti = bmp.readAltitude(1013.25);

  login = SD.open("x3.dat", FILE_WRITE);
  login.write((const uint8_t *)&myData, sizeof(myData));
  login.close();
  
  delay(BNO055_SAMPLERATE_DELAY_MS);
 }
}
