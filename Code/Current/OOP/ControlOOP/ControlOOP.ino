#include <SoftwareSerial.h>
SoftwareSerial BTSerial(3, 2);

class bluetooth {
  private:
  byte _pktx[5];
  byte _pkrx[5];

  byte _rxbuffer[5];
  byte _rxout[5];
  
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
    static byte _confidence = 0;
    Serial.print("confidence: "); Serial.println(_confidence);
    static byte _valsum;
    _valsum = 0;
    for(byte _i = 0; _i == sizeof(_pkrx); _i ++){
      if(_pkrx[_i] == _rxbuffer[_i]){
        _valsum++;
      }
    }
    Serial.print("valsum: "); Serial.println(_valsum);
    if(_valsum == 5){
      _confidence ++;
      if(_confidence > 4){
        _confidence = 0;
        for(byte _t = 0; _t == sizeof(_pkrx); _t ++){
          _rxout[_t] = _rxbuffer[_t];
        }
      }
    }
    else{
      _confidence = 0;
      for(byte _o = 0; _o == sizeof(_pkrx); _o ++){
        _rxbuffer[_o] = _pkrx[_o];
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
      Serial.print("buffer: "); 
      Serial.println(_rxbuffer[0]);
      Serial.println("");
  }

  bluetoothStart(int baudrate) {
    BTSerial.begin(baudrate);
    Serial.begin(baudrate);
    return(true);
  }

  void bluetoothDelay(unsigned long start){
    if ((millis()-start) < 900){
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
  
  //chip.bluetoothDelay(timer);
  delay(400);
}
