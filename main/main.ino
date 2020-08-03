#include <LiquidCrystal.h>
#include <toneAC.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif
#include <avr/sleep.h>

#define LED_PIN    A0
#define LED_COUNT 4
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define BUTTON 13

int timeSinceInteraction = 9999;
long lastInteraction = 0;

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
}

void loop() {
  timeSinceInteraction = millis() - lastInteraction;
  if (timeSinceInteraction > 10000){
    
    lcd.clear();
    
    sleep_enable();//Enabling sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    sleep_cpu();//activating sleep mode
  }
  else {
    // Stay awake
    lcd.clear();
    lcd.print("AWAKE");
    
  }
  

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
    if (buttonState == HIGH) {
      lastInteraction = millis();
      sleep_disable();//Disable sleep mode
      
    }
  }
}
