#include <SoftwareSerial.h>
SoftwareSerial BTSerial(3, 2);

class shift{
  private:
  byte _clk;
  byte _latch;
  byte _data;
  byte _pinstates[8];
  bool _blinking;
  unsigned long _lastBlink;

  info(byte searchVal){
    int _info = 0;
    for(byte _i = 0; _i < sizeof(_pinstates); _i ++){
      if(_pinstates[_i] == searchVal || _pinstates[_i] == 3){
        _info |= (1 << _i);
      }
    }
    Serial.println(_info);
    return(_info);
  }

  public:

  void deploy(byte _a){
    digitalWrite(_latch, LOW);
    shiftOut(_data, _clk, MSBFIRST, info(_a));
    digitalWrite(_latch, HIGH);
  }

  void shiftStart(byte clk, byte latch, byte data){
    _clk = clk;
    _latch = latch;
    _data = data;

    byte _pinstates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
    pinMode(_latch, OUTPUT);
    pinMode(_clk, OUTPUT);
    pinMode(_data, OUTPUT);
    digitalWrite(_latch, HIGH);
  }

  void setState(byte pin, byte state){
    if(_pinstates[pin] != state){
      _pinstates[pin] = state;
    }
  }

  void blinkUpdate(unsigned long currentTime){
    if((currentTime - _lastBlink) > 1000){
      _lastBlink = currentTime;
      for(byte _i = 0; _i < sizeof(_pinstates); _i ++){
        if(_pinstates[_i] == 3){
          _pinstates[_i] = 4;
        }
        else if(_pinstates[_i] == 4){
          _pinstates[_i] = 3;
        }        
      }
      deploy(1);
    }
  }
};

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

    /*
      Serial.print("tx: "); 
      Serial.println(_pktx[0]);
  Serial.print("rx: "); 
  Serial.println(_pkrx[0]);
      Serial.print("buffer: "); 
      Serial.println(_rxbuffer[0]);
      Serial.println("");*/
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

shift led1;

unsigned long timer;

void setup() {
  pinMode(8, OUTPUT);
  // clock latch data 
  chip.bluetoothStart(9600);

  led1.shiftStart(11, 10, 12);
  pinMode(9, INPUT);
  led1.deploy(1);

  led1.setState(0, 3);
  led1.setState(8, 0);
  led1.deploy(1);
}

void loop() {
  delay(300);

  
  timer = millis();
  chip.bluetoothRun();

  if(digitalRead(9)){
    //chip.bluetoothSet(0, 1);
  }
  else{
    chip.bluetoothSet(0, 0);
  }


  if(chip.rxInfo(0) == 1){
    digitalWrite(8, HIGH);
  }

  led1.blinkUpdate(millis());
  
  delay(50);
  
}
