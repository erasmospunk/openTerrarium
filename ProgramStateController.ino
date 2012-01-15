/**************************************************
 * class: ProgramStateController
 * constructor: initProgramStateController()
 *
 * methods:
 **************************************************/

#include <EEPROM.h>
#include "Settings.h"

char selection;
extern char cursorPos;
#define SCREEN_TIMEOUT 30

void noOperation(boolean isPressed){}

void autocontrolScreenReset() {
  if (Global.lastUserInteraction + Settings.getLcdTimeout() + SCREEN_TIMEOUT < now()) {
    loadMainScreen();
  }
}


void upPressedImpl(boolean isPressed) {
  increaseLcdBrightness(10);
}
void downPressedImpl(boolean isPressed) {
  increaseLcdBrightness(-10);
}
void leftPressedImpl(boolean isPressed) {
  increaseTargetTemp(-0.5);
}
void rightPressedImpl(boolean isPressed) {
  increaseTargetTemp(0.5);
}
void enterPressedImpl(boolean isPressed) {
//  printChar();
  loadMenuScreen();
}

void logicMainScreen() {
//  printButton();

  printDateTime(0);
//  printTargetTemp(1);
  printTempAnimation(tempC);
  printRelay();
}

void loadMainScreen() {
  if (logic == logicMainScreen) {//if loaded
    return;
  }
  
  upPressed = &upPressedImpl;
  downPressed = &downPressedImpl;
  leftPressed = &leftPressedImpl;
  rightPressed = &rightPressedImpl;
  enterPressed = &enterPressedImpl;
  
  logic = &logicMainScreen;
  
  clearScreen();
}





void upPressedMenuImpl(boolean isPressed) {
  moveCurUp();
}
void downPressedMenuImpl(boolean isPressed) {
  moveCurDown();
}
void leftPressedMenuImpl(boolean isPressed) {
  loadMainScreen();
}
void rightPressedMenuImpl(boolean isPressed) {
  enterPressedMenuImpl(isPressed);
}
void enterPressedMenuImpl(boolean isPressed) {
  switch (cursorPos) {
  case 0:
    loadTempSetupScreen();
    break;
  case 1:
    loadTimeSetupScreen();
    break;
  case 2:
    loadAboutScreen();
    break;
  }

}

void loadMenuScreen() {
  upPressed = &upPressedMenuImpl;
  downPressed = &downPressedMenuImpl;
  leftPressed = &leftPressedMenuImpl;
  rightPressed = &rightPressedMenuImpl;
  enterPressed = &enterPressedMenuImpl;
    
  //cursorPos = 0;
  
  logic = &drawMenuUi;
  
  loadMenu(1);
  clearScreen();
}





byte selSec;
#define TEMP_STR_SIZE 8
#define EDIT_ITEMS 2
#define ITEM_1_LINE 1
#define ITEM_2_LINE 3
void upAndDown(char dir) {
  if (selSec == 0) { //select high and low
    eraseCur(0, selection);
    selection += -2 * dir;
    if (selection < ITEM_1_LINE) {
      selection = ITEM_1_LINE;
    }
    else if (selection > ITEM_2_LINE) {
      selection = ITEM_2_LINE;
    }
    drawCur(0, selection);
  }
  else if (selSec == 1){ //edit temp
    if (selection == 1)
      Settings.setMaxTargetTemp(Settings.getMaxTargetTemp() + 10 * dir);
    else
      Settings.setMinTargetTemp(Settings.getMinTargetTemp() + 10 * dir);
  }
  else if (selSec == 2){ //edit hours
    if (selection == 1) {
      Settings.setMaxTargetTimeHour(Settings.getMaxTargetTimeHour() + dir);
    }
    else {
      Settings.setRelayOnDayPercent(Settings.getRelayOnDayPercent() + 0.011 * dir);
    }
//    else
//      Settings.setMinTargetTimeHours(Settings.getMinTargetTimeHours() + dir);
  }
//  else if (selSec == 3){ //edit minutes
//    if (selection == 1)
//      Settings.setMaxTargetTimeMinutes(Settings.getMaxTargetTimeMinutes() + dir);
//    else
//      Settings.setMinTargetTimeMinutes(Settings.getMinTargetTimeMinutes() + dir);
//  }
}

void cleanSelection() {
  if (selSec == 1){
    eraseSelection(0, TEMP_STR_SIZE, selection);
  }
  else if (selSec == 2){
    eraseSelection(11, 14, selection);
  }  
//  else if (selSec == 3){
//    eraseSelection(16, 19, selection);
//  }
}

void upPressedTempSetupImpl(boolean isPressed) {
  upAndDown(1);
}
void downPressedTempSetupImpl(boolean isPressed) {
  upAndDown(-1);
}
void leftPressedTempSetupImpl(boolean isPressed) {
  if (selSec == 0) {//exit if not editing temp
    loadMenuScreen();
    return;
  }
  else {
    cleanSelection();
  }
  
  selSec--;
  
}
void rightPressedTempSetupImpl(boolean isPressed) {
  if (selSec != EDIT_ITEMS) {
    cleanSelection();
    selSec++;
  }
}
void enterPressedTempSetupImpl(boolean isPressed) {
  if (selSec == EDIT_ITEMS) {
    Settings.saveConfig();
    Settings.debugConfig();
    loadMenuScreen();
    return;
  }
  else {
    cleanSelection();
    selSec++;
  }
}

#define TEMP_STR_SIZE 8
void editTempLogic() {
  printTempNumber(1, 1, Settings.getMaxTargetTempFloat());
  printTempNumber(1, 3, Settings.getMinTargetTempFloat());
  
  printInt(12, 1, Settings.getMaxTargetTimeHour());
  printFloat(12, 3, Settings.getRelayOnDayPercent());
  
  if (selSec == 0) {
    drawCur(0, selection);
  }
  else if (selSec == 1){
    drawSelection(0, TEMP_STR_SIZE, selection);
  }
  else if (selSec == 2){
    if (selection == 1) {
      drawSelection(11, 14, selection);
    }
    else {
      drawSelection(11, 16, selection);
    }
    
  }
}

void loadTempSetupScreen() {
  upPressed = &upPressedTempSetupImpl;
  downPressed = &downPressedTempSetupImpl;
  leftPressed = &leftPressedTempSetupImpl;
  rightPressed = &rightPressedTempSetupImpl;
  enterPressed = &enterPressedTempSetupImpl;

  selection = ITEM_1_LINE;
  selSec = 0;
  
  logic = &editTempLogic;
  
  clearScreen();
}






#define TIME_FIELDS 6
time_t editTime;
boolean timeChanged = false;

void changeTime(char by) {
  TimeElements tm;
  breakTime(editTime, tm);
  
  switch(selection) {
  case 0:
    tm.Hour += by;
    if ((char)tm.Hour < 0) {
      tm.Hour = 23;
    }
    tm.Hour %= 24;
    break;
  case 1:
    tm.Minute += by;
    if ((char)tm.Minute < 0) {
      tm.Minute = 59;
    }
    tm.Minute %= 60;
    break;
  case 2:
    tm.Second += by;
    if ((char)tm.Second < 0) {
      tm.Second = 59;
    }
    tm.Second %= 60;
    break;
  case 3: tm.Day += by; break;
  case 4: tm.Month += by; break;
  case 5: tm.Year += by; break;
  }
  
  editTime = makeTime(tm);
}

void upPressedTimeSetupImpl(boolean isPressed) {  
  changeTime(1);
  timeChanged = true;
}
void downPressedTimeSetupImpl(boolean isPressed) {
  changeTime(-1);
  timeChanged = true;
}
void leftPressedTimeSetupImpl(boolean isPressed) {
  if (selection == 0) {
    timeChanged = false;
    enterPressedTimeSetupImpl(true);
  }
  printTimeSelector(selection, selection - 1, 2);
  
  selection--;
}
void rightPressedTimeSetupImpl(boolean isPressed) {
  if (selection == TIME_FIELDS - 1) {
    enterPressedTimeSetupImpl(true);
  }
  printTimeSelector(selection, selection + 1, 2);
  
  selection++;
}
void enterPressedTimeSetupImpl(boolean isPressed) {
  if (timeChanged) {
    setTime(editTime);
    RTC.set(editTime);
  }
  loadMenuScreen();
}

void timeSetupScreenLogic() {
  printDateTime(1, editTime);
}

void loadTimeSetupScreen() {
  upPressed = &upPressedTimeSetupImpl;
  downPressed = &downPressedTimeSetupImpl;
  leftPressed = &leftPressedTimeSetupImpl;
  rightPressed = &rightPressedTimeSetupImpl;
  enterPressed = &enterPressedTimeSetupImpl;
  
  selection = 0;
  
  editTime = now();
  
  logic = &timeSetupScreenLogic;
  
  clearScreen();
  
  printTimeSelector(selection, selection, 2);
}













void exitPressedAboutImpl(boolean isPressed) {
  loadMenuScreen();
}

void loadAboutScreen() {
  upPressed = &noOperation;
  downPressed = &noOperation;
  leftPressed = &exitPressedAboutImpl;
  rightPressed = &noOperation;
  enterPressed = &noOperation;
  
  logic = &printAboutScreen;
  
  clearScreen();
}
