#include <SoftwareSerial.h>
SoftwareSerial BTSerial(3, 2);

class bluetooth {
  private:
  byte _pktx[5];
  byte _pkrx[5];

  byte _rxbuffer[5];
  byte _rxout[5];
  byte _confidence;
  
  void bluetoothTransmit() {
    BTSerial.write(_pktx, sizeof(_pktx));
  }

  void bluetoothRecive() {
    static byte _count = 10;
    if(BTSerial.available() >= sizeof(_pkrx)){
      BTSerial.readBytes(_pkrx, sizeof(_pkrx));
      while(BTSerial.available() > 0)
        BTSerial.read();   
      bluetoothTransmit();
    }
    else {
      if (_count >=10) {
        _count = 0;

        bluetoothTransmit();
      }
      _count++;
    }
  }

  void valueCheck(){
    Serial.println(_pkrx[0]);
    byte _rxsize;
    _rxsize = sizeof(_pkrx);
    static byte _valsum = 0;
    
    for(byte _i = 0; _i < _rxsize; _i ++){
      if(_pkrx[_i] == _rxbuffer[_i]){
        _valsum++;
      }
    }
    if(_valsum == 5){
      _confidence ++;
      if(_confidence > 15){
        _confidence = 0;
        for(byte _t = 0; _t < _rxsize; _t ++){
          _rxout[_t] = _rxbuffer[_t];
        }
      }
    }
    else{
      _confidence = 0;
      for(byte _o = 0; _o < _rxsize; _o ++){
        _rxbuffer[_o] = _pkrx[_o];
      }
    }
    _valsum = 0;
  }

  public:

  void bluetoothRun(){
    bluetoothRecive();
    valueCheck();
  }

  bluetoothStart(int baudrate) {
    BTSerial.begin(baudrate);
    Serial.begin(baudrate);
    return(true);
  }

  void bluetoothDelay(unsigned long start){
    if ((millis()-start) < 60){
      delay(millis()-start);
    }
  }

  void bluetoothSet(byte spot, byte data){
    _pktx[spot] = data;
  }

  rxInfo(byte spot){
    return(_rxout[spot]);
  }
};

bluetooth chip;

unsigned long timer;

void setup() {
  chip.bluetoothStart(115200);

  pinMode(8, OUTPUT);
  pinMode(9, INPUT);
}

void loop() {
  timer = millis();
  chip.bluetoothRun();

  if(digitalRead(9)){
    chip.bluetoothSet(0, 1);
  }
  else{
    chip.bluetoothSet(0, 0);
  }


  if(chip.rxInfo(0) == 1){
    digitalWrite(8, HIGH);
  }

  delay(50);
  //chip.bluetoothDelay(timer);
}
