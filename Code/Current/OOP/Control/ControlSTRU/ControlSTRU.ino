#include <SoftwareSerial.h>
SoftwareSerial BTSerial(3, 2);

class bluetooth {
  private:
  
  byte tx[1];
  byte rx[1];
  
  void bluetoothRecive() {
    static byte count = 10;
    if(BTSerial.available() >= sizeof(rx)){
      BTSerial.readBytes(rx,sizeof(rx));
      while(BTSerial.available() > 0)
        BTSerial.read();   
      bluetoothTransmit();
    }
    else {
      if (count >=10) {
        count = 0;

        bluetoothTransmit();
      }
      count++;
    }
  }

  void bluetoothTransmit() {
    BTSerial.write(tx,sizeof(tx));
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
    //valueCheck();

    Serial.print("tx: "); 
    Serial.println(pkt_tx.a);
    Serial.print("rx: "); 
    Serial.println(pkt_rx.a);
  }

  bluetoothStart(byte baudrate) {
    BTSerial.begin(baudrate);
    return(true);
  }

  void bluetoothDelay(unsigned long start){
    if ((millis()-start) < 55){
      delay(millis()-start);
    }
  }

  void bluetoothSet(byte data){
    pkt_tx.a = data;
  }

  rxInfo(byte spot){
    return(pkt_rx.a);
  }
};

bluetooth chip;

unsigned long timer;

void setup() {
  //chip.bluetoothStart(9600);

  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.print("running");

  pinMode(8, OUTPUT);
  pinMode(9, INPUT);
}

void loop() {
  timer = millis();
  //chip.bluetoothRun();

  bluetoothRecive();

  if(digitalRead(9)){
    tx[0] = 1;
  }
  else{
    tx[0] = 0;
  }


  if(rx[0] == 1){
    digitalWrite(8, HIGH);
  }
  else if (rx[0] == 0){
    digitalWrite(8, LOW);
  }  
    Serial.print("tx: "); 
    Serial.println(tx[0]);
    Serial.print("rx: "); 
    Serial.println(rx[0]);
  delay(50);
  //chip.bluetoothDelay(timer);
}
