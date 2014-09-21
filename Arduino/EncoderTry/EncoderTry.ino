#include <Serial.h>

volatile int p2 = 1;
volatile int p3 = 1;
volatile int counter = 0;

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(13, INPUT);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(13, HIGH);
  attachInterrupt(0, doEncoder, CHANGE); 
  attachInterrupt(1, doEncoder, CHANGE); 
  Serial.begin(9600);
  Serial.println("begin");
}

void loop() {
  delay(10000);
}

void doEncoder(){
  int p2n = digitalRead(2);
  int p3n = digitalRead(3);
  int a = analogRead(0);
  Serial.println(digitalRead(13));
  //Serial.println(p2n);
  //Serial.println(a);

  if (p2 != p2n) {
    if (p2n == 1) {
      if (p3n == 0) {
        counter--;
      } else {
        counter++;
      }
    } else {
      if (p3n == 0) {
        counter++;
      } else {
        counter--;
      }
    }
  } else if (p3 != p3n) {
    if (p3n == 1) {
      if (p2n == 0) {
        counter++;
      } else {
        counter--;
      }
    } else {
      if (p2n == 0) {
        counter--;
      } else {
        counter++;
      }
    }
  }

  p2 = p2n;
  p3 = p3n;

  if (counter > 3) {
    counter = 0;
    Serial.println("r");
  } else if (counter < -3) {
    counter = 0;
    Serial.println("l");
  }
}
