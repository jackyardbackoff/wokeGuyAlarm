/* pierre's tme code */
/* updates every second. alarm (internal LED) off for 5s. */

// why no wifi?
// -> because i'm in student accommodation with user-specific logins fuck

#include <ESP32Time.h>

//ESP32Time rtc;
ESP32Time rtc(3600);  // offset in seconds GMT+1

int count = 0; 
int hold = 5; // max.

// buffers for set-alarm time and real time
int hours = 23;
int minutes = 31;
char alarmTime[6]; // HH:MM -> string buffer or character array
char currentTime[6]; // HH:MM

int alarmState = 0; // 0 - off, 1 - set, 2 - on.


void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  rtc.setTime(55, 30, 22, 21, 11, 2024);  // 21st Nov 2024, 23:30:20 -> for some reason, offset by one hour.
}

void loop() {
  Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));   // (String) returns time with specified format 

  struct tm timeinfo = rtc.getTimeStruct();
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");   //  (tm struct) Sunday, January 17 2021 07:24:38
  sprintf(alarmTime, "%02d:%02d", hours, minutes);
  sprintf(currentTime, "%02d:%02d", rtc.getHour(true), rtc.getMinute()); // true in rtc's gethour() for 24hr format.

  Serial.println(alarmTime);
  Serial.println(currentTime);
  Serial.println();

  if (strcmp(alarmTime, currentTime) == 0 && alarmState == 2) { // 0 means same, 1 means not same
    Serial.println("Boom!");
    digitalWrite(2, HIGH);
  }
  digitalWrite(2, LOW);

  delay(1000); // 1 second delay.
}
