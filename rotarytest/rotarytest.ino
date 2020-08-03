#include <Encoder.h>
#include <LiquidCrystal.h>

Encoder myEnc(3, 2);
const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(20, 4);
  lcd.display();
}

long position  = -999;

void loop() {
  long newPos = myEnc.read();
  lcd.clear();
  lcd.print(String(newPos));
  
  if (newPos != position) {
    position = newPos;
  }
  
  delay(50);
}
