// test pia 12/12 for little lcd screen

// used uno. sketch for alarm clock
// display time and messages (date, alarm state, sound state)

// width default = 6 when txt is 1. height default 8.
// 128wide * 160tall


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS        10
#define TFT_RST        8 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);



void setup() {
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB);
  tft.setFont();
  tft.setCursor(0,0);
  tft.setRotation(90);
  tft.fillScreen(ST77XX_BLACK);

}

void loop() {
  
  //
  tft.setRotation(-45); // to make display on line...
  tft.setTextColor(ST77XX_WHITE);
  // display primary TIME !
  tft.setTextSize(5);
  tft.setCursor(8, 25);
  tft.print("21:04");
  
  //tft.print("SOUND CHANGED   (2/5)");
  printCenteredText("setting alarm....  01:55", 1, 77);
  printCenteredText("A-> hr, B-> min, C-> set", 1, 90);
  /*
  printCenteredText("setting alarm....  01:10");
  tft.setCursor(8, 87);
  tft.print("A-> hr, B-> min, C-> set");
  */
  
  //printCenteredText("Friday 28 January", 1, 77); // text, size, yval

  delay(5000);
  tft.fillScreen(ST77XX_BLACK);
}


// function to center text (horizontally -><-)
void printCenteredText(const char* text, uint8_t textSize, uint8_t yCursor) {
  tft.setTextSize(textSize);
  tft.setTextColor(ST7735_WHITE); // Set text color
  tft.setTextWrap(false);         // Disable text wrapping

  int16_t x1, y1;
  uint16_t textWidth;
  uint16_t textHeight; // don't use this but need for adafruit's function 

  // Measure text dimensions
  tft.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);

  // Calculate position to center text
  int16_t x = (tft.width() - textWidth) / 2;

  // Set cursor to the calculated position and display text
  tft.setCursor(x, yCursor);
  tft.print(text);
}