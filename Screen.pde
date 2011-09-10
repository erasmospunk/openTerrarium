
// include the library code:
#include <ShiftRegLCD.h>

#include "Settings.h"

#include "Constants.h"

#define TEMP_STR_SIZE 7 //after print temp, move position 8 chars: i.e. 29.56* C

#define MENU_CURSOR_CHAR (char)126
#define TEMP_DEGREE_CHAR (char)223

#define MAX_MENU_SIZE 16

#define MAIN_MENU 1

/**************************************************
 * class: Lcd
 * constructor: initLcd()
 *
 * methods:
 **************************************************/


ShiftRegLCD lcd(LCD_DATA_PIN, LCD_CLOCK_PIN, LCD_ENABLE_PIN, LCD_LINES);




int lcdBrightness;
char linePos = 0;
char bufferLen = 0;
char* buffer[MAX_MENU_SIZE];

//lcd lines fix
static int scr = 0;
void toggleScr() {
  scr = (scr == 2) ? 0 : 2;
}

void initLcd() {
  pinMode(LCD_BRIGHTNESS_PIN, OUTPUT);
  
  turnOnLcdBrightness();

  //lcd.blink();
}

void increaseLcdBrightness(int byFactor) {
  lcdBrightness = Settings.getLcdBrightness();
  lcdBrightness += byFactor;
  if (lcdBrightness > 255) {
    lcdBrightness = 255;
  }
  else if (lcdBrightness < LCD_MIN_BRIGHTNESS) {
    lcdBrightness = LCD_MIN_BRIGHTNESS;
  }
  
  Settings.setLcdBrightness(lcdBrightness);
  setLcdBrightness(lcdBrightness);
}

void setLcdBrightness(int b) {
  lcdBrightness = b;
  analogWrite(LCD_BRIGHTNESS_PIN, b);
}

void turnOnLcdBrightness() {
  lcdBrightness = Settings.getLcdBrightness();
  setLcdBrightness(lcdBrightness);
}

void autocontrolLcdBrightness() {
  if (Global.lastUserInteraction + Settings.getLcdTimeout() < now()) {
    int hourNow = hour();
    int lcdMinBrightness = 0;
    if (hourNow > 5 && hourNow < 24) {
      lcdMinBrightness = LCD_MIN_BRIGHTNESS;
    }
    
    if (lcdBrightness > lcdMinBrightness) {
      setLcdBrightness(--lcdBrightness);
    }
    else if (lcdBrightness < lcdMinBrightness){
      setLcdBrightness(++lcdBrightness);
    }
  }
  else {
    turnOnLcdBrightness();
  } 
}


void clearScreen() {
  lcd.clear();
}

#define MAIN_MENU 1
void loadMenu(byte menuNumber) {
 switch (menuNumber) {
  case MAIN_MENU:
    buffer[0] = "Set temperatures   ";
    buffer[1] = "Time and date      ";
    buffer[2] = "About              ";
    bufferLen = 3;
   break; 
  case 99:
    buffer[0] = "menu0              ";
    buffer[1] = "menu1              ";
    buffer[2] = "menu2              ";
    buffer[3] = "menu3              ";
    buffer[4] = "menu4              ";
    buffer[5] = "menu5              ";
    buffer[6] = "menu6              ";
    bufferLen = 7;
   break; 
 }
  
}



void printString(byte colPos, byte linePos, char *str) {
  lcd.setCursor(colPos, (linePos+scr)%LCD_LINES);
  lcd.print(str);
}

void printInt(byte colPos, byte linePos, int i) {
  lcd.setCursor(colPos, (linePos+scr)%LCD_LINES);
  lcd.print(i);
}

void printFloat(byte colPos, byte linePos, float f) {
  lcd.setCursor(colPos, (linePos+scr)%LCD_LINES);
  lcd.print(f);
}

byte uiState = 0;
boolean uiStateChanged = true;

void menuHandler() {
  
  switch (uiState) {
    case 1:
      if (uiStateChanged) {
        loadMenu(99);
        uiStateChanged = false;
      }
      if (menuChangeAnimRight()) {
        uiState = 0;
        uiStateChanged = true;
      }
      else { return; }
      break;
    case 2:
      if (uiStateChanged) {
        loadMenu(MAIN_MENU);
        uiStateChanged = false;
      }
      if (menuChangeAnimLeft()) {
        uiState = 0;
        uiStateChanged = true;
      }
      else { return; }
      break;
  }
}


char cursorPos = 0;
void moveCurUp() {
  if (cursorPos != 0) {
    lcd.setCursor(0, (cursorPos+scr)%LCD_LINES);
    lcd.print(" ");
    cursorPos--;
    drawCur();
  }
  else {
    scrollScreenUp();
  }
}

void moveCurDown() {
  if (cursorPos != LCD_LINES-1 && cursorPos != bufferLen-1) {
    eraseCur(0, cursorPos);
    cursorPos++;
    drawCur();
  }
  else {
    scrollScreenDown();
  }
}

void drawCur() {
  drawCur(0, cursorPos);
}

void drawCur(char colPos, char linePos) {  
  for(int i = 0; i < LCD_LINES; i++) {
    lcd.setCursor(colPos, (i+scr)%LCD_LINES);
    if (i == linePos) {
      lcd.print(MENU_CURSOR_CHAR);
    }
    else {
      lcd.print(" ");
    }
  }
}

void eraseCur(char colPos, char linePos) {
  lcd.setCursor(colPos, (linePos+scr)%LCD_LINES);
  lcd.print(" ");
}

void drawSelection(char colPosStart, char colPosEnd, char linePos) {  
  lcd.setCursor(colPosStart, (linePos+scr)%LCD_LINES);
  lcd.print("[");
  lcd.setCursor(colPosEnd, (linePos+scr)%LCD_LINES);
  lcd.print("]");
}

void eraseSelection(char colPosStart, char colPosEnd, char linePos) {  
  lcd.setCursor(colPosStart, (linePos+scr)%LCD_LINES);
  lcd.print(" ");
  lcd.setCursor(colPosEnd, (linePos+scr)%LCD_LINES);
  lcd.print(" ");
}

#define MENU_START_POS 1

void drawMenuUi() {
  //Serial.println("-----redraw");
  
  for(int i = 0; i != LCD_LINES && i != bufferLen; i++) {
    lcd.setCursor(MENU_START_POS, (i+scr)%LCD_LINES);
    lcd.print(buffer[i + linePos]);
//    Serial.print((i+scr)%LCD_LINES);
//    Serial.print(" ");
//    Serial.print(" ");
//    Serial.println(buffer[i + linePos]);
  }
  
  drawCur();
 
//  Serial.println();
//  Serial.println();
}

void scrollScreenUp() {
  if (linePos > 0) {
    linePos--;
    drawMenuUi();
  }
}

void scrollScreenDown() {
  if (linePos < bufferLen - 4) {
    linePos++;
    drawMenuUi();
  }
}

/*
  Enter a menu. returns true when animation finished
*/
boolean menuChangeAnimRight() {
  static char pos = 0;
  
  if (pos == 0) {//if begining animation
    toggleScr();
  }
  
  lcd.scrollDisplayLeft();
  for(int i = 0; i < LCD_LINES; i++) {
    lcd.setCursor(pos + 1,(i+scr)%LCD_LINES);
    if (i != bufferLen) { //don't print empty menu lines
      lcd.print(buffer[i][pos]);
    }
    else {
      lcd.print(" ");
    }
  }  
  //drawCur();
  
  pos++;
    
  drawCur();
    
  if (pos == LCD_LINE_SIZE) {//if anim finished
    pos = 0;
    linePos = 0;    //reset menu line
    return true;
  }
  else {
    return false;
  }
}

/*
  leave a menu. returns true when animation finished
*/
boolean menuChangeAnimLeft() {
  static char pos = LCD_LINE_SIZE - 1;
  
  if (pos == LCD_LINE_SIZE - 1) {//if begining animation
    toggleScr();
  }
  
  lcd.scrollDisplayRight();
  for(int i = 0; i < LCD_LINES; i++) {
    lcd.setCursor(pos,(i+scr)%LCD_LINES);

    if (i != bufferLen) { //don't print empty menu lines
      lcd.print(buffer[i][pos-1]);
    }
    else {
      lcd.print(" ");
    }
  }
  drawCur();
  
  pos--;
  
  if (pos == -1) {//if anim finished
    pos = LCD_LINE_SIZE - 1;
    linePos = 0;    //reset menu line  
    return true;
  }
  else {
    return false;
  }
}

void printRelay() {
  lcd.setCursor(19, 1);
  if (relayStatus) ////external var from Relay
    lcd.print("R");
  else
    lcd.print(" ");
}

void printChar() {
  lcd.setCursor(6, 0);
  static byte c;
  lcd.print(c, DEC);
  lcd.print(" ");
  lcd.print(c++);
  lcd.print("  ");
}

void printAboutScreen() {
  lcd.setCursor(8, 1);
  lcd.print("Hot");
  lcd.setCursor(6, 2);
  lcd.print("Reptile");
  lcd.setCursor(8, 3);
  lcd.print(VERSION);
}

void printButton() {
  lcd.setCursor(15, 0);
  
  digitalWrite(BUTTONS_A_PIN,0);
  digitalWrite(BUTTONS_B_PIN,0);
  digitalWrite(BUTTONS_C_PIN,0);
  int v = digitalRead(BUTTONS_INPUT_PIN);  
  if (v == HIGH) {
    lcd.print("O");
  } else {lcd.print(".");}
//  Serial.print(v); Serial.print(" ");
  
  digitalWrite(BUTTONS_A_PIN,1);
  digitalWrite(BUTTONS_B_PIN,0);
  digitalWrite(BUTTONS_C_PIN,0);
  v = digitalRead(BUTTONS_INPUT_PIN);
  if (v == HIGH) {
    lcd.print("O");
  } else {lcd.print(".");}
//  Serial.print(v); Serial.print(" ");
  
  digitalWrite(BUTTONS_A_PIN,0);
  digitalWrite(BUTTONS_B_PIN,1);
  digitalWrite(BUTTONS_C_PIN,0);
  v = digitalRead(BUTTONS_INPUT_PIN); 
  if (v == HIGH) {
    lcd.print("O");
  } else {lcd.print(".");}
//  Serial.print(v); Serial.print(" ");
  
  digitalWrite(BUTTONS_A_PIN,1);
  digitalWrite(BUTTONS_B_PIN,1);
  digitalWrite(BUTTONS_C_PIN,0);
  v = digitalRead(BUTTONS_INPUT_PIN); 
  if (v == HIGH) {
    lcd.print("O");
  } else {lcd.print(".");}
//  Serial.print(v); Serial.print(" "); 
  
  digitalWrite(BUTTONS_A_PIN,0);
  digitalWrite(BUTTONS_B_PIN,0);
  digitalWrite(BUTTONS_C_PIN,1);
  v = digitalRead(BUTTONS_INPUT_PIN); 
  if (v == HIGH) {
    lcd.print("O");
  } else {lcd.print(".");}
  //Serial.println(v);
}

void printTargetTemp(byte lineNumber) {
  lcd.setCursor(0, lineNumber);
  lcd.print("Target: ");
  printTempNumber(getTargetTemp());
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

void printDateTime(byte lineNumber) {
  printDateTime(lineNumber, now());
}

void printDateTime(byte lineNumber, time_t time) {
  printDateTime(lineNumber, hour(time), minute(time), second(time), day(time), month(time), year(time));
}

void printDateTime(byte lineNumber, int hr, int m, int sec, int day, int month, int yr) {
  // set the cursor to column 0, line 'lineNumber'
  lcd.setCursor(0, lineNumber);
  
  printDigits(hr);
  lcd.print(":");
  printDigits(m);
  lcd.print(":");
  printDigits(sec);
  
  lcd.setCursor(10, lineNumber);
  printDigits(day);//read date
  lcd.print("/");
  printDigits(month);//read month
  lcd.print("/");
  printDigits(yr); //read year
}

byte getTimeSelectorColumn(byte pos) {
  byte col;
  switch (pos) {
    case 0:
      col = 1;
      break;
    case 1:
      col = 4;
      break;
    case 2:
      col = 7;
      break;
    case 3:
      col = 11;
      break;
    case 4:
      col = 14;
      break;
    case 5:
      col = 19;
      break;
  }
  
  return col;
}
      
void printTimeSelector(byte oldPos, byte newPos, byte lineNumber) {
  printSelector(getTimeSelectorColumn(oldPos), getTimeSelectorColumn(newPos), lineNumber);
}

void printSelector(byte oldPos, byte newPos, byte lineNumber) {
  lcd.setCursor(oldPos, lineNumber);
  
  lcd.print(" ");

  lcd.setCursor(newPos, lineNumber);
  
  lcd.print("^");
}


void printTimeSetupScreen() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  printDigits(hour());
  lcd.print(":");
  printDigits(minute());
  lcd.print(":");
  printDigits(second());
  
  lcd.setCursor(10, 1);
  printDigits(day());//read date
  lcd.print("/");
  printDigits(month());//read month
  lcd.print("/");
  printDigits(year()); //read year
}


float lastTemp = 0.0;
#define DISPLAY_TEMP_RANGE 0.2
void printTempAnimation(float tempC) {  
  int increasingDirection = 0;
  
  //print only temperatures that differ at least 0.2C
  if ( tempC > (lastTemp + DISPLAY_TEMP_RANGE) || tempC < (lastTemp - DISPLAY_TEMP_RANGE)) {
    if (tempC > lastTemp) {
      increasingDirection = 1;
    }
    else if (tempC < lastTemp) {
      increasingDirection = -1;
    }
    
    lastTemp = tempC;
  }
  
  int cursorPosition = graphTemp(lastTemp, increasingDirection);
  
  if (cursorPosition > 14) {
    cursorPosition = 14;
  }
  
  
  lcd.setCursor(0, 2);
  //clear any character before the temperature number
  for(int i = 0; i<cursorPosition; i++) {
   lcd.print(" ");
  }
  

  printTempNumber(tempC);
  cursorPosition += TEMP_STR_SIZE;

  //clear any chars after temp, if needed
  if (cursorPosition < 20) {
    for(; cursorPosition < 20; cursorPosition++) {
      lcd.print(" ");
    }
  }
}


void printTempNumber(float tempC) {
  lcd.print(tempC, 1);
  lcd.print(TEMP_DEGREE_CHAR);
  lcd.print("C");
}

void printTempNumber(byte colPos, byte linePos, float tempC) {
  lcd.setCursor(colPos, linePos);
  printTempNumber(tempC);
}


int tempStabiled = 0;
int lastDirection = 0;
int graphTemp(int temp, int increasingDirection) {
  if (temp > 40) {
    temp = 40;
  }
  
  int level = map(temp, 0, 40, 0, 19);

  
  if (increasingDirection > 0) {
    tempStabiled = 1000/PROGRAM_SPEED; //hold this direction for 1 sec
    lastDirection = 1;
  }
  else if (increasingDirection < 0) {
    tempStabiled = 1000/PROGRAM_SPEED; //hold this direction for 1 sec
    lastDirection = -1;
  }
  else {
    if (tempStabiled > 0) {
      tempStabiled--;
    }
    else {
      lastDirection = 0;
    }
  }
    
  char printChar;  
  if (lastDirection > 0) {
    printChar = '>';
  }
  else if (lastDirection < 0) {
    printChar = '<';    
  }
  else {
    printChar = '*';
  }

  lcd.setCursor(0, 3);
  for(int i=0; i!=level; i++) {
    lcd.print(printChar);
  }
  
  for(int i = level; i != 20; i++) {
    lcd.print(" ");
  }
    
  return level;   
}
