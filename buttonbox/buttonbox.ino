// display logic

#include <TM1637Display.h>

#define CLK 2
#define DIO 3

// var to display the word 'FLAP' 
const uint8_t FLAP[] = {
  SEG_A | SEG_E | SEG_F | SEG_G,                   // F
  SEG_D | SEG_E | SEG_F,                           // L
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,   // A
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,           // P
  };


// variable for displaying "----"
const uint8_t LINE[] = {
  SEG_G,                                           // G
  SEG_G,                                           // G
  SEG_G,                                           // G
  SEG_G,                                           // G
  };
  

TM1637Display display(CLK, DIO);



// button logic

const int button = 4;
boolean buttonState = HIGH;
int tapCount = 0;
unsigned long debounce = 0;
unsigned long inputTimer = 0;


// SETUP
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //button mode INPUT_PULLUP to take advantage of internal resitor to force 'HIGH' state
  pinMode(button, INPUT_PULLUP);
    
}



// LOOP
void loop() {

  //debounce logic
  if (debounce < millis()) {
    
    if(digitalRead(button) == LOW && buttonState == HIGH) {
      tapCount++;
      buttonState = LOW;
      debounce = (millis() + 10);
      
      if (inputTimer == 0) {
        inputTimer = (millis() + 2000);
      }     
      
      
      // for testing button taps/count in serial port
//      Serial.println("buttonState = " + String(buttonState));
      Serial.println("tapCount = " + String(tapCount));
      
    } else if (digitalRead(button) == HIGH && buttonState == LOW) {
      buttonState = HIGH;
      delay(10);
    }

    //tap rate results display logic
    if ((inputTimer > 0) && (inputTimer < millis())) {
      
      float tapRate = ((int(tapCount) * 100) / 2);
      
      display.showNumberDecEx(tapRate, 0b01000000, false, 4, 0);

      Serial.println("final tapCount = " + String(tapCount));
      Serial.println("tapRate = " + String(tapRate / 100));

      // display final result blinking effect
      for(int i = 0; i < 3; i++) {
        
        display.showNumberDecEx(tapRate, 0b01000000, false, 4, 0);
        delay(300);
        display.clear();
        delay(300);
        
      }
      
      inputTimer = 0;
      tapCount = 0;

    }
  }


  //display logic

  
  // set display brightness
  display.setBrightness(0x0f);

  //display LINE before tapcount is 0, and FLAP while 2 second timer is active
  if(tapCount == 0) {
    display.setSegments(LINE);
  } else {
    display.setSegments(FLAP);
//    display.showNumberDec(int(tapCount));
  }

  
  
}
