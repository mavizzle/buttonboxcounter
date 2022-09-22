#include <ezButton.h>
#include <TM1637Display.h>

#define CLK 2
#define DIO 3

ezButton button(7);  // create ezButton object that attaches to pin 7;

unsigned long timer = 0;

const uint8_t FLAP[] = {
  SEG_A | SEG_E | SEG_F | SEG_G,                   // F
  SEG_D | SEG_E | SEG_F,                           // L
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,   // A
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,           // P
  };


TM1637Display display(CLK, DIO);


void setup() {
  // serial plotter for testing
  Serial.begin(9600);
  
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
  button.setCountMode(COUNT_FALLING); // count when button state goes from HIGH to LOW
  
}

void loop() {

  button.loop(); // MUST call the loop() function first

  // set brightness
  display.setBrightness(0x0f);

  
//  //display FLAP before timer is running
  if(int(timer) == 0) {
    display.setSegments(FLAP);
  }
  
 
  
  

  unsigned long count = button.getCount(); // store button pressed count
  
  if(button.isPressed()){

    // print button presses to serial monitor
    Serial.println("tap count:" + String(count));

    // start 2 second timer window for recording taps
    if(int(timer)==0) {
      timer = (millis() + 2000);
      display.clear();
    }
  }

   // display countdown while timer is running
  if (timer > millis()) {
    
    int countDown = (int(timer) - int(millis()));
    
//    Serial.println(countDown);
    display.showNumberDecEx(countDown, 0b10000000, false, 4, 1);
  }

 


  
  // end of timer button taprate math & timer reset statement
  if ((timer > 0) && (timer < millis())) {
   
    int tapRate = ((int(count) * 1000) / 20);

    
    Serial.println(float(tapRate) / 100); // show taprate in serial monitor

    // show final taprate on number display

    for (int i = 0; i < 3; i++) {
      display.showNumberDecEx(tapRate, 0b01000000, false, 4, 0);
      delay(300);
      display.clear();
      delay(300);
    }
    delay(300);
    timer = 0;
    button.resetCount();
    
  }

}
