#include <Wire.h>
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
    byte _rxsize;
    _rxsize = sizeof(_pkrx);
    Serial.print("confidence: "); Serial.println(_confidence);
    static byte _valsum = 0;
    
    for(byte _i = 0; _i < _rxsize; _i ++){
      if(_pkrx[_i] == _rxbuffer[_i]){
        _valsum++;
      }
    }
    Serial.print("valsum: "); Serial.println(_valsum);
    if(_valsum == 5){
      _confidence ++;
      if(_confidence > 4){
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

class hardwire {
  private:
  byte _txid;
  byte _tx;
  byte _rx;
  

  void _hardwireReceive(int bytes){
    _rx = Wire.read();
  }

  public:
  void hardwireStart(byte txid){
    _txid = txid;
    Wire.onReceive(_hardwireReceive);
  }

  hardwireQuery(byte val){
    _tx = val;
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(_tx); 
    Wire.endTransmission();  
  }

  hardwireRx(){
    return(_rx);
  }
};

hardwire pixel;

void setup() {
  chip.bluetoothStart(9600);
  pixel.hardwireStart(8);

  pinMode(8, OUTPUT);
  pinMode(9, INPUT);

}

unsigned long millis();

void loop() {
  timer = millis();
  
  chip.bluetoothRun();
  
  if(pixel.hardwireRx() == 5){
    chip.bluetoothSet(0, 1);
  }
  else{
    chip.bluetoothSet(0, 0);
  }

  if(chip.rxInfo(0) == 1){
    digitalWrite(8, HIGH);
    hardwireQuery(3);
  }
  else if (chip.rxInfo(0) == 0){
    digitalWrite(8, LOW);
  }

  chip.bluetoothDelay(timer);
}
