class shift{
  private:
  byte _clk;
  byte _latch;
  byte _data;
  byte _pinstates[8];
  bool _blinking;
  unsigned long _lastBlink;

  void deploy(byte _a){
    digitalWrite(_latch, LOW);
    shiftOut(_data, _clk, MSBFIRST, info(_a));
    digitalWrite(_latch, HIGH);
  }

  info(byte searchVal){
    byte _info = 0;
    for(byte _i = 0; _i < 9; _i ++){
      if(_pinstates[_i] == searchVal || _pinstates[_i] == 3){
        _info |= (1 << _i);
      }
    }
    return(_info);
  }

  public:

  void shiftStart(byte clk, byte latch, byte data){
    _clk = clk;
    _latch = latch;
    _data = data;

    byte _pinstates[] = {0, 0, 0, 0, 0, 0, 0, 0};
  
    pinMode(_latch, OUTPUT);
    pinMode(_clk, OUTPUT);
    pinMode(_data, OUTPUT);
  }

  void setState(byte pin, byte state){
    if(_pinstates[pin] != state){
      _pinstates[pin] = state;
      deploy(1);
    }
  }

  void blinkUpdate(unsigned long currentTime){
    if((currentTime - _lastBlink) > 1000){
      Serial.println(_pinstates[1]);
      _lastBlink = currentTime;
      for(byte _i = 0; _i < 9; _i ++){
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

shift maine;

void setup() {
  maine.shiftStart(11, 10, 12);

  Serial.begin(9600);

  maine.setState(1, 3);
  maine.setState(0, 1);
  maine.setState(2, 3);

}

void loop() {
  maine.blinkUpdate(millis());
}
