#include <LiquidCrystal.h>
#include <toneAC.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif
#include <avr/sleep.h>
#include <Encoder.h>

#define LED_PIN    A0
#define LED_COUNT 4
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define BUTTON 13

int timeSinceInteraction = 9999;
long lastInteraction = 0;

int rotarySelection = 0;
int baseLine = 0;
Encoder myEnc(2, 3);

int menuIndex = 0;






void setup() {

  lcd.begin(20, 4);
  lcd.display();
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();          
  strip.show();            
  strip.setBrightness(100); 
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.setPixelColor(1, strip.Color(0, 0, 0));
  strip.setPixelColor(2, strip.Color(0, 0, 0));
  strip.setPixelColor(3, strip.Color(0, 0, 0));

  strip.show();

  pinMode(BUTTON, INPUT);
  pciSetup(BUTTON);
  updateScreen();
}

void loop() {
  delay(50);
  
  
  
  
  timeSinceInteraction = millis() - lastInteraction;
  if (timeSinceInteraction > 10000){
    
    lcd.clear();
    
    sleep_enable();//Enabling sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    sleep_cpu();//activating sleep mode
  }
  else {
    long newPos = myEnc.read();
    lcd.setCursor(15, 0);
     lcd.print(String(newPos));
      if (newPos != rotarySelection) {
        if (abs(rotarySelection - newPos) > 5) {
          lastInteraction = millis();
          rotarySelection = floor(newPos/25.0);
          updateScreen();
        }
        
      }
    
  }
  

}

void updateScreen() {
  lcd.clear();
  String options[10] = {"Back", "Back", "Back", "Back", "Back", "Back", "Back", "Back", "Back", "Back"};
  if (menuIndex == 0) {
    options[0] = "Option 1";
    options[1] = "Option 2";
  }
  else if (menuIndex == 1) {
    options[0] = "SubOption1";
    options[1] = "SubOption2";
  }

  int optionCount = 0;
  int startIndex = 0;
  int endIndex = 0;

  
  
  for (int i = 0; i < 10; i++) {
    if (options[i] == "Back") {
      optionCount = i+1;
      break;
    }
  }

  if ((rotarySelection - baseLine) > optionCount) {
    baseLine = (rotarySelection - baseLine) - optionCount;
  }

  if (optionCount <=4) {
    endIndex = optionCount-1;
  }
  else {
    int selection = (rotarySelection - baseLine);
    if (selection <=4) {
      endIndex = 4;
    }
    else {
      startIndex = selection;
      endIndex = startIndex+4;
    }
  }

  for (int i = startIndex; i < endIndex+1;i++) {
    lcd.setCursor(0, i-startIndex);
    if (i == (rotarySelection - baseLine)) {
      lcd.print(">");
    }
    lcd.print(options[i]);

  }
  lcd.setCursor(4, 20);
  

}


void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
 {    
     interruptTriggered(1);
 }
 
ISR (PCINT1_vect) // handle pin change interrupt for A0 to A5 here
 {
     interruptTriggered(2);
 }  
 
ISR (PCINT2_vect) // handle pin change interrupt for D0 to D7 here
 {
     interruptTriggered(0);
 }  
 

void interruptTriggered(int i) {
  if (i == 1) {
    bool buttonState = digitalRead(BUTTON);
      lastInteraction = millis();
      sleep_disable();//Disable sleep mode
      
  }
}
