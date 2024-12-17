int hours = 0;
int mins = 0;
int sound = 0;
int alarmState = 0; 


void setup() {
  Serial.begin(115200);
  pinMode(26, INPUT_PULLUP); // HOURBUTT
  pinMode(27, INPUT_PULLUP); // MINBUTT
  pinMode(14, INPUT_PULLUP); // SOUNDBUTT
  pinMode(25, INPUT_PULLUP); // alarmBUTT
}

void loop() {
  Serial.print("HOUR: ");
  Serial.print(digitalRead(26));
  Serial.print(" | MIN: ");
  Serial.print(digitalRead(27));
  Serial.print(" | SOUND: ");
  Serial.print(digitalRead(14));
  Serial.print(" | ALARM: ");
  Serial.println(digitalRead(25));
  delay(100); // .1 second
}