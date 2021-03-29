// Include the required Wire library for I2C<br>
#include <Wire.h>

int LED = 5;
int x = 0;

int buttonid = 4;

int state = LOW;

long timer = 0;
long buttondelay = 500 ;

int buttonPin = 7;

void setup() {
  Serial.begin(115200);
  
  // Define the LED pin as Output
  pinMode (LED, OUTPUT);
  pinMode (buttonPin, INPUT);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}
void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C
  
}
void loop() {

  state = digitalRead(buttonPin);
  Serial.print(state);

    if (state && millis() - timer > buttondelay){
    Serial.print("sending");
    timer= millis();

    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(buttonid);             
    Wire.endTransmission();
    Wire.begin(9);
  }
  
  //If value received is 0 blink LED for 200 ms
  if (x == 0) {
    digitalWrite(LED, HIGH);
    delay(900);
    digitalWrite(LED, LOW);
    delay(900);
  }
  //If value received is 3 blink LED for 400 ms
  if (x == 1) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
}
