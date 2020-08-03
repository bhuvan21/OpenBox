#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(20, 4);
  lcd.print("hello, world!");
}

void loop() {
  lcd.noBlink();
  delay(3000);
  lcd.blink();
  delay(3000);
}
