/**************************************************
 * class: Buttons
 * constructor: initLcd()
 *
 * methods:
 **************************************************/


//up .O...
//down O....
//left ....O
//right ...O.
//enter ..O..

#define NUM_OF_BUTTONS 5

#include "Constants.h"

void initButtons() {
  
  pinMode(BUTTONS_INPUT_PIN, INPUT);    
  pinMode(BUTTONS_A_PIN, OUTPUT);
  pinMode(BUTTONS_B_PIN, OUTPUT);
  pinMode(BUTTONS_C_PIN, OUTPUT);

}

void callButton(char buttonNumber, boolean isPressed) {
  static unsigned char lastPress[NUM_OF_BUTTONS];
  
  if (buttonFilter(isPressed, &(lastPress[buttonNumber]))) { return; }
  
  switch(buttonNumber) {
  case 0:
    downPressed(isPressed);
    break;
  case 1:
    upPressed(isPressed);
    break;
  case 2:
    enterPressed(isPressed);
    break;
  case 3:
    rightPressed(isPressed);
    break;
  case 4:
    leftPressed(isPressed);
    break;
  }
}
void buttons() {
  boolean isPressed;
  for(char n = 0; n != NUM_OF_BUTTONS; n++) {
    digitalWrite(BUTTONS_A_PIN, bitRead(n, 0));
    digitalWrite(BUTTONS_B_PIN, bitRead(n, 1));
    digitalWrite(BUTTONS_C_PIN, bitRead(n, 2));
    isPressed = digitalRead(BUTTONS_INPUT_PIN);

    if (isPressed) {
      Global.lastUserInteraction = now();
    }
    
    callButton(n, isPressed);
  }
  
}

boolean buttonFilter(boolean isPressed, unsigned char *lastPress) {
  if (!isPressed) {
    *lastPress = 0;
    return true;
  }
  
  if (*lastPress > 0) {
    (*lastPress)--;
    return true;
  }  
  *lastPress = 1000/BUTTONS_SPEED/PROGRAM_SPEED;

  return false;
}
