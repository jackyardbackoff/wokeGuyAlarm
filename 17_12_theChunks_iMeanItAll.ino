/* 
  pia 17/12 - code for ALARM, buttons. putting on button controller.
  -> if sd card not working, have to unplug replug.

*/

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(17, 16); // RX and TX pins for esp.
DFRobotDFPlayerMini player; // dfplayer inst.

// buttons
#define ALARMBUTT 25
#define HOURBUTT 26
#define MINBUTT 27
#define SOUNDBUTT 14

// led pins
#define LED_R 12
#define LED_G 13
#define LED_B 33
int leds[] = {LED_R, LED_G, LED_B};

// readings are hex vals (high = 1111, low = 000)
int thisButtRead[] = {LOW, LOW, LOW, LOW};
int lastButtRead[] = {LOW, LOW, LOW, LOW};
int butts[] = {ALARMBUTT, HOURBUTT, MINBUTT, SOUNDBUTT};

// debounce is for delay ...
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
const unsigned long debounceDelay = 50;

int alarmState = 2; // tri-state alarm var
int hours = 0;
int mins = 0;

int sound = 2; // 6-state sound var
int numSounds = 2;
bool playing = false;




void setup() {
  // setup serial & mp3
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("initializing DFplayer"));
  if (!player.begin(mySoftwareSerial)) {
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    while (true);
  }
  Serial.println();
  Serial.println(F("DFPlayer initialized GJ"));

  player.volume(5); // max vol YES
  player.EQ(0); // equalizer because i'm woke

  // set up buttons
  for (int i = 0; i < sizeof(butts) / sizeof(butts[0]); i++) { // why this  ? -> BYTES
    pinMode(butts[i], INPUT_PULLUP);
  }

  // setup LED
  for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++) {
    pinMode(leds[i], OUTPUT);
  }
  alarmChangeState();

  Serial.print(sound); // current sound.
}


void loop() {
  // in loop : check all buttons to see if action is required.
  // buttons -> must push-release to register, no infinite holds.

  // get all readings - external loop checks ALL BUTTONS using this
  for (int i = 0; i < sizeof(butts) / sizeof(butts[0]); i++) {
    int reading = digitalRead(butts[i]);

  // check for debouncing - push-release
    if (reading != lastButtRead[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      // If the button state has changed
      if (reading != thisButtRead[i]) {
        thisButtRead[i] = reading;

        // Only trigger on button press (active-low)
        if (thisButtRead[i] == LOW) {
          buttPressed(i);
        }
      }
    }

    // set 0-1 dependent.
    lastButtRead[i] = reading;

    // CHECK ALARM STATE and COMPARE THE TIME;
    if (alarmState == 2) {
      if (playing == false) {
        Serial.println("Alarm started.");
        playing = true;
        player.loop(sound); // disabled in alarmChangeState().
      }
    }
  }




  /*
  Serial.println(alarmState);
  //soundChange();
  if (alarmState == 2) {
    player.loop(sound);
  }

  delay(10000); // 10 seconds

  if (count == 4) {
    alarmState = 0;
    player.stop(); // emulates effect of alarm button press / state change
  }

  count = (count + 1) % 5; */
}



void buttPressed(int buttIndex) {
  // switch and perform fn based on butt index + states. Uses index.
  switch (buttIndex) {
    case 0: // Alarm button
      alarmChangeState();
      break;
    case 1: // Hour button
      changeTime(HOURBUTT);
      // change line to display.
      break;
    case 2: // Minute button
      changeTime(MINBUTT);
      break;
    case 3: // Sound button
      soundChange();
      break;
  }
}

void alarmChangeState() {
  // change state of the alarm (inactive -> setting -> active ->...)
  alarmState = (alarmState + 1) % 3;
  Serial.println(alarmState);

  if (alarmState == 0) {
    player.pause();
    playing = false; // turn off alarm
    Serial.println(F("Alarm inactive."));
    setColor(0, 0, 0); // inactive
  }
  else if (alarmState == 1) {
    Serial.println(F("SETTING ALARM | increment hours-mins, and press ALARM again to set."));
    Serial.print(F("Current alarm set to: "));
    Serial.print(hours);
    Serial.print(":");
    Serial.print(mins);
    Serial.println();
    setColor(255, 125, 0); // set - yellow
  }
  else {
    Serial.println(F("Alarm active | watchaself"));
    // set alarm sbuffer, display on LCD screen.
    Serial.print(hours);
    Serial.print(":");
    Serial.print(mins);
    Serial.println();
    setColor(175, 0, 0); // red - active
  }
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(LED_R, 0 + redValue);
  analogWrite(LED_G, 0 + greenValue);
  analogWrite(LED_B, 0 + blueValue);
}


void changeTime(int pin) {
  if (alarmState != 1) {
    Serial.println("Not in set mode, can't change time.");
    return;
  }

  if (pin == MINBUTT) {
    mins = (mins + 5) % 60;
  }
  if (pin == HOURBUTT) {
    hours = (hours + 1) % 24;
  }
  Serial.print(hours);
  Serial.print(":");
  Serial.print(mins);
  Serial.println();
}

void soundChange() {
  if (alarmState == 2) {
    // messes with alarm looping.
    Serial.println("Bro you can't do that right now!");
    return;
  }
  
  // change the sound, based on limit
  sound = (sound % numSounds) + 1; // 1 more cause thats how deef works i guess
  player.play(sound);

  Serial.println(F("Changed alarm sound to file "));
  Serial.println(sound);

  // save string to variable, display sound change for 10 seconds. then revert.
}