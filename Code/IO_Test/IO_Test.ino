

void setup() {
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

}

void loop() {
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

  delay(2000);
}
