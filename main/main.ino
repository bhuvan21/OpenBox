#include <LiquidCrystal.h>
#include <toneAC.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif
#include <avr/sleep.h>
#include <Encoder.h>
#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>

#define LED_PIN    A3
#define LED_COUNT 4
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define BUTTON 13

int timeSinceInteraction = 9999;
long lastInteraction = 0;

int rotarySelection = 0;
int baseLine = 0;
Encoder myEnc(3, 2);

int menuIndex = 0;


bool sleeping = false;

int navigation[2][10] = {{1,2,0,0,0,0,0,0,0,0}, {0,3,0,0,0,0,0,0,0}};

volatile bool flag = false;

long pressTime;

long lightsOff;


bool foilHome2Flag = false;
volatile bool hitFlags1[] = {false};

volatile bool hitFlagChange1 = false;
volatile bool hitFlagRise1 = false;
volatile bool hitFlagFall1 = false;


volatile bool skipMain = false;
volatile bool interRead;

void setup() {
  Serial.begin(2000000);
  Serial.println("Started.");

  
  strip.begin();
  strip.show();
  strip.setBrightness(100);
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.setPixelColor(1, strip.Color(0, 0, 0));
  strip.setPixelColor(2, strip.Color(0, 0, 0));
  strip.setPixelColor(3, strip.Color(0, 0, 0));


  strip.show();
  
  lastInteraction = millis();
  lcd.begin(20, 4);
  lcd.display();


  

  pinMode(BUTTON, INPUT);
  updateScreen();

  pinMode(A0, INPUT);
  enableInterrupt(A0, f2, CHANGE);
  
  enableInterrupt(BUTTON, middlePressed, RISING);
}



void loop() {
  long startWait = millis();

  // WAIT 50ms but break out if an interrupt happened and something needs to be done
  while (millis()-startWait < 50) {
    if (hitFlagChange1) {
      break;
    }
  }

  // DETERMINE RISE OR FALL FOR F2
  if (hitFlagChange1) {
    if (interRead) {
      hitFlagRise1 = true;
      hitFlagFall1 = false;
      Serial.println("Rising Detected");
    }
    else {
      hitFlagFall1 = true;
      hitFlagRise1 = false;
      Serial.println("Falling Detected");
    }

    if (menuIndex == 3) {
      // HANDLE RISE AND FALLS for F2
      if (hitFlagRise1) {
        Serial.println("LEDs on unconditionally, so is piezo.");
        setLightsNoStop(true, false, false, false);
        pressTime = millis();
        hitFlagRise1 = false;
        hitFlagChange1 = false;
        //toneAC(2000);
      }
      if (hitFlagFall1) {
        if (millis() - pressTime >= 15){
          Serial.println("Valid Hit.");
          if (millis() - pressTime <= 2000) {
            Serial.println("Short hit, keep lights on");
            setLights(true, false, false, false);
          }
          else {
            lightsOff = millis();
            Serial.println("Long hit, go off.");
          }
          
          
        }
        else {
          lightsOff = millis();
          Serial.println("Flicker, go off.");
        }
        hitFlagFall1 = false;
        hitFlagChange1 = false;
        
      }
    }





    
  }


  Serial.print("Lights off: ");
  Serial.println(lightsOff);


  if (millis() >= lightsOff) {
    Serial.println("Lights and piezo off.");
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.setPixelColor(1, strip.Color(0, 0, 0));
    strip.setPixelColor(2, strip.Color(0, 0, 0));
    strip.setPixelColor(3, strip.Color(0, 0, 0));
    //toneAC(0);
    strip.show();
    
  }

  if (!skipMain) {

    lcd.setCursor(13, 3);
    lcd.print(millis());
  
    
    
  
    
    timeSinceInteraction = millis() - lastInteraction;
    if (timeSinceInteraction > 10000){
      
      lcd.clear();
      sleeping = true;
      sleep_enable();//Enabling sleep mode
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
      sleep_cpu();//activating sleep mode
    }
    else {
      long newPos = myEnc.read();
  
        if (newPos != rotarySelection) {
          if (abs(rotarySelection*20 - newPos) >= 20.0) {
            lastInteraction = millis();
            rotarySelection = floor(newPos/20.0);
            updateScreen();
          }
          
        }
      
    }
    
  }
  
  
  

}

void updateScreen() {
  lcd.clear();
  String options[10] = {"Back", "Back", "Back", "Back", "Back", "Back", "Back", "Back", "Back", "Back"};
  if (menuIndex == 0) {
    options[0] = "At Home Foil";
    options[1] = "Information";
  }
  else if (menuIndex == 1) {
    options[0] = "Flick Testing";
    options[1] = "Regular Testing";
  }
  else if (menuIndex == 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Info Page");
    return;
  }
  else if (menuIndex == 3) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Regular Testing Mode");
    lcd.setCursor(0, 3);
    lcd.print("ON!");
    skipMain = true;
    return;
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

  if ((rotarySelection - baseLine) > optionCount-1) {
    baseLine = (rotarySelection+1) - optionCount;
  }

  if ((rotarySelection - baseLine) < 0) {
    baseLine = rotarySelection;
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

void menuSelect() {
  skipMain = false;
  int basics[] = {2, 3};
  bool flag = false;
  for (int i = 0; i<2; i++) {
    if (basics[i] == menuIndex) {
      flag = true;
    }
  }
  
  if (flag) {
    menuIndex = 0;
  }
  else {
    menuIndex = navigation[menuIndex][rotarySelection-baseLine];
    baseLine = rotarySelection;
  }
  updateScreen();
}



void setLights(bool a, bool b, bool c, bool d) {
  if (a) {
    strip.setPixelColor(0, strip.Color(255, 0, 0));
  }
  
  if (b) {
    strip.setPixelColor(0, strip.Color(255, 255, 255));
  }

  if (c) {
    strip.setPixelColor(0, strip.Color(255, 255, 255));
  }

  if (d) {
    strip.setPixelColor(0, strip.Color(0, 0, 255));
  }
  strip.show();

  lightsOff = millis() + 2000;
}

void setLightsNoStop(bool a, bool b, bool c, bool d) {
  if (a) {
    strip.setPixelColor(0, strip.Color(255, 0, 0));
  }
  
  if (b) {
    strip.setPixelColor(0, strip.Color(255, 255, 255));
  }

  if (c) {
    strip.setPixelColor(0, strip.Color(255, 255, 255));
  }

  if (d) {
    strip.setPixelColor(0, strip.Color(0, 0, 255));
  }
  strip.show();
  lightsOff = millis() + 999999;
}



void middlePressed() {
  if (sleeping) {
  
    bool buttonState = digitalRead(BUTTON);
    lastInteraction = millis();
    sleeping = false;
    updateScreen();
    sleep_disable();//Disable sleep mode
  }
  else {
    menuSelect();
  }
}

void f2() {
    if (menuIndex == 3) {
      hitFlagChange1 = true;
      interRead = digitalRead(A0) == HIGH;
    lastInteraction = millis();
    sleeping = false;
    sleep_disable();
    }
    
  
      
}
