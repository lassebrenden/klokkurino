/**
* Include libraries
*/
#include <Time.h>
#include <TimeAlarms.h>
#include <LiquidCrystal.h>

/**
* Initiate LiquidCrystal library
*/
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/**
* Set constants
*/
const unsigned int buttonMode = 7;
const unsigned int buttonUp = 8;
const unsigned int buttonDown = 9;
const unsigned int piezo = 6;

/**
* Set variables
*/
unsigned int buttonModeState = 0;
unsigned int buttonUpState = 0;
unsigned int buttonDownState = 0;
unsigned int piezoState = 0;
unsigned int hours = 0;
unsigned int minutes = 0;
unsigned int seconds = 0;
boolean hoursIsSet = false;
boolean minutesIsSet = false;
boolean secondsIsSet = false;

/**
* Read and store button states
*/
void readButtonStates() {
  buttonModeState = digitalRead(buttonMode);
  buttonUpState = digitalRead(buttonUp);
  buttonDownState = digitalRead(buttonDown);
}

/**
* Print digits with leading zero
*/
void printDigits(int digit) {
  if(digit < 10) {
    lcd.print('0');
  }

  lcd.print(digit);
}

/**
* Display the current time
*/
void displayCurrentTime() {
  lcd.clear();
  printDigits(hour());
  lcd.print(':');
  printDigits(minute());
  lcd.print(':');
  printDigits(second());
}

/**
* Setup program
*/
void setup() {
  // Set pin modes
  pinMode(buttonMode, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);

  // Set dimensions of LCD
  lcd.begin(16, 2);
}

/**
* Run program
*/
void loop() {
  // Read button states
  readButtonStates();

  // Has time been set?
  if(timeStatus() == timeSet) {
    displayCurrentTime();
  } else {
    // Set hours
    if(!hoursIsSet) {
      // Clear display
      lcd.clear();

      // Print instructions
      lcd.print("--:--:--");
      lcd.setCursor(0, 1);
      lcd.print("Set hours: 00");
      lcd.setCursor(11, 1);

      // Listen for button input
      while(!hoursIsSet) {
        // Read button states
        readButtonStates();

        // Process button states
        if(buttonModeState == HIGH) {
          hoursIsSet = true;
          delay(100);
        } else if(buttonUpState == HIGH) {
          if(hours < 23) {
            hours++;
          }
        } else if(buttonDownState == HIGH) {
          if(hours > 0) {
            hours--;
          }
        }

        // Reset hours
        lcd.print("  ");

        // Set cursor
        lcd.setCursor(11, 1);

        // Print hours
        printDigits(hours);

        // Delay button
        delay(100);
      }

      // Set hours
      if(hoursIsSet) {
        // Set cursor
        lcd.setCursor(0, 0);

        // Print hours
        printDigits(hours);
      }
    }

    // Set minutes
    if(!minutesIsSet) {
      // Print instructions
      lcd.setCursor(0, 1);
      lcd.print("Set minutes: 00");
      lcd.setCursor(13, 1);

      // Listen for button input
      while(!minutesIsSet) {
        // Read button states
        readButtonStates();

        // Process button states
        if(buttonModeState == HIGH) {
          minutesIsSet = true;
          delay(100);
        } else if(buttonUpState == HIGH) {
          if(minutes < 59) {
            minutes++;
          }
        } else if(buttonDownState == HIGH) {
          if(minutes > 0) {
            minutes--;
          }
        }

        // Reset minutes
        lcd.print("  ");

        // Set cursor
        lcd.setCursor(13, 1);

        // Print minutes
        printDigits(minutes);

        // Delay button
        delay(100);
      }

      // Set minutes
      if(minutesIsSet) {
        // Set cursor
        lcd.setCursor(3, 0);

        // Print minutes
        printDigits(minutes);
      }
    }

    // Set seconds
    if(!secondsIsSet) {
      // Print instructions
      lcd.setCursor(0, 1);
      lcd.print("Set seconds: 00");
      lcd.setCursor(13, 1);

      // Listen for button input
      while(!secondsIsSet) {
        // Read button states
        readButtonStates();

        // Process button states
        if(buttonModeState == HIGH) {
          secondsIsSet = true;
          delay(100);
        } else if(buttonUpState == HIGH) {
          if(seconds < 59) {
            seconds++;
          }
        } else if(buttonDownState == HIGH) {
          if(seconds > 0) {
            seconds--;
          }
        }

        // Reset seconds
        lcd.print("  ");

        // Set cursor
        lcd.setCursor(13, 1);

        // Print seconds
        printDigits(seconds);

        // Delay button
        delay(100);
      }

      // Set seconds
      if(secondsIsSet) {
        // Set cursor
        lcd.setCursor(6, 0);

        // Print seconds
        printDigits(seconds);
      }
    }

    // Clear second line
    lcd.setCursor(0, 1);
    lcd.print("                ");

    // Print confirmation
    lcd.setCursor(0, 1);
    lcd.print("Time is set");

    // Set time
    setTime(hours, minutes, seconds, 1, 1, 2013);
  }

  // Delay 1 second
  delay(1000);
}
