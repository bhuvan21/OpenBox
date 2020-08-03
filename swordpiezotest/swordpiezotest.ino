unsigned long buttonPressedAt = 0;
const int BUTTON = A3;
bool buttonReleased = false;
byte buttonPrevState = 0;

#include <toneAC.h>

void setup() {
 pinMode(BUTTON,INPUT);
}

void loop() {

  int state = analogRead(BUTTON) > 900;
  
  if (state != buttonPrevState) {
    if (state == HIGH) {
      buttonReleased = false;
      buttonPressedAt = millis();
    }
    else {
      buttonReleased = true;
    }
    buttonPrevState=state;
  }

  if (buttonReleased == true) {
    toneAC(2000);
    delay(1000);
    toneAC();
    
    buttonReleased=false;
    buttonPressedAt = 0;
  }
}
