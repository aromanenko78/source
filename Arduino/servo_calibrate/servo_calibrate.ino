#include <Servo.h> 

#define SERVO_PIN 9 // HEAT
//#define SERVO_PIN 10 // FLOW

//#define HEAT_CLICKS { 0, 13, 25, 36, 47, 62, 75, 88, 100, 114, 129, 143, 154, 174 }
//#define HEAT_CLICKS { 0, 7, 17, 27, 39, 53, 65, 77, 90, 103, 115, 128, 139, 150 } // corrected

//#define FLOW_CLICKS { 10, 37, 48, 59, 78, 90, 103, 115, 128, 141 }
//#define FLOW_CLICKS { 10, 37, 48, 63, 77, 89, 101, 114, 131, 145 } // corrected

String str;

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  checkSerial();
}

void checkSerial() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      processCommand(str);
      str = "";
    } else {
      str += c;
    }
  }
}

void processCommand(String str) {
  setServo(str.toInt());
}

void setServo(float angle) {
  Serial.print("Setting servo to ");
  Serial.println(angle);
  Servo servo;
  servo.attach(SERVO_PIN);
  servo.write(angle);
  delay(2000);
  servo.detach();
}


