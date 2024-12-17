// pia 17/12
// sound testing with esp32
// looping , checking sd card works.
// if sd card not working, have to unplug replug.
// had to add variable to CONFIRM PLAYING or not. Prob is way on component but IDGAF

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(17, 16); // RX and TX pins for esp.
DFRobotDFPlayerMini player; // dfplayer inst.

int sound = 1; // 6-state sound var
int numSounds = 2;

int alarmState = 2;
int count = 0;

bool playing = false;

void setup() {
  // setup serial & music
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
  player.EQ(0); // idk music theory but this is an equalizer
}

void loop() {
  Serial.println(alarmState);
  //soundChange();
  if (alarmState == 2) {
    if (playing == false) {
      Serial.println("helloGirlBoy");
      playing = true;
      player.loop(sound);
    }
  }

  delay(100); // 10 seconds

  if (count == 99) {
    alarmState = 0;
    playing = false;
    player.stop(); // emulates effect of alarm button press / state change
  }

  count = (count + 1) % 100;
  Serial.println(count);
}

// sound things
void soundChange() {
  sound = (sound % numSounds) + 1; // 1 more cause thats how deef works i guess
  player.play(sound);

  Serial.println(F("Changed alarm sound to file "));
  Serial.println(sound);
}