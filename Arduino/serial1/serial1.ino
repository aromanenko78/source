String str;
int led = 12;

void setup() {
  pinMode(led, OUTPUT); 
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  str = "";
  while (true) {
    while (!Serial.available()) {
      delay(1);
    }
    char c = Serial.read();
    if (c == '\n') {
      break;
    } else {
      str += c;
    }
  }
  
  if (str.length() > 0) {
    process(str);
  }
}

void process(String str) {
  if (str == "on") {
    digitalWrite(led, HIGH);
  } else if (str == "off") {
    digitalWrite(led, LOW);
  } else {
    // echo back
    Serial.println(str);
  }
}
