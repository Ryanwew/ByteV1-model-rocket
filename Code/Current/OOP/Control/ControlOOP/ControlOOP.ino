#include <SoftwareSerial.h>
SoftwareSerial BTSerial(3, 2);

class bluetooth {
  private:
  byte _pktx[5];
  byte _pkrx[5];

  byte _rxbuffer[5];
  byte _rxout[5];
  
  void bluetoothTransmit() {
    BTSerial.write(_pktx, 5);
  }

  void bluetoothRecive() {
    static byte _count = 10;
    if(BTSerial.available() >= 5){
      BTSerial.readBytes(_pkrx, 5);
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
    static byte _confidence = 0;
    
    if(_pkrx == _rxbuffer){
      _confidence ++;
      if(_confidence > 5){
        for(byte _i = 0; _i < 6; _i ++){
          _rxout[_i] = _rxbuffer[_i];
        }
      }
    }
    else{
      _confidence = 0;
      for(byte _i = 0; _i < 6; _i ++){
        _rxbuffer[_i] = _pkrx[_i];
      }
    }
  }

  public:

  void bluetoothRun(){
    bluetoothRecive();
    valueCheck();

      Serial.print("tx: "); 
      Serial.println(_pktx[0]);
  Serial.print("rx: "); 
  Serial.println(_pkrx[0]);
  }

  bluetoothStart(int baudrate) {
    BTSerial.begin(baudrate);
    return(true);
  }

  void bluetoothDelay(unsigned long start){
    if ((millis()-start) < 55){
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
  chip.bluetoothStart(9600);

  Serial.begin(9600);
  Serial.print("running");

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
  else if (chip.rxInfo(0) == 0){
    digitalWrite(8, LOW);
  }  
  
  chip.bluetoothDelay(timer);
}
