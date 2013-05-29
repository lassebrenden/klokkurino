/**
* Include libraries
*/
#include <Bounce.h>
#include <LiquidCrystal.h>
#include <MP3.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <Timer.h>

/**
* Set constants
*/
// Set the time to delay between each button press
const unsigned int BUTTON_BOUNCE_DELAY = 50;

// Set the servo pin
const unsigned int PIN_SERVO = 6;

// Set the button pins
const unsigned int PIN_BUTTON_BLUE = 7;
const unsigned int PIN_BUTTON_GREEN = 8;
const unsigned int PIN_BUTTON_RED = 9;
const unsigned int PIN_BUTTON_YELLOW = 10;

// Set the LED pins
const unsigned int PIN_LED_BLUE = 15;
const unsigned int PIN_LED_GREEN = 16;
const unsigned int PIN_LED_RED = 17;
const unsigned int PIN_LED_YELLOW = 18;

// Set the pin for randomSeed (must be analog and unused)
const unsigned int PIN_RANDOM = A0;

/**
* Declare variables
*/
// Whether or not the buttons have changed its state since the last button press
boolean buttonBlueChangedState = false;
boolean buttonGreenChangedState = false;
boolean buttonRedChangedState = false;
boolean buttonYellowChangedState = false;

// Whether or not the alarm has been set and/or enabled
boolean isAlarmEnabled = false;
boolean isAlarmHoursSet = false;
boolean isAlarmMinutesSet = false;
boolean isAlarmSet = false;

// Whether or not the time has been set
boolean isTimeHoursSet = false;
boolean isTimeMinutesSet = false;
boolean isTimeSecondsSet = false;
boolean isTimeSet = false;

// The button bouncer objects (prevents the buttons from being pressed multiple times in succession)
Bounce buttonBlueBouncer = Bounce(PIN_BUTTON_BLUE, BUTTON_BOUNCE_DELAY);
Bounce buttonGreenBouncer = Bounce(PIN_BUTTON_GREEN, BUTTON_BOUNCE_DELAY);
Bounce buttonRedBouncer = Bounce(PIN_BUTTON_RED, BUTTON_BOUNCE_DELAY);
Bounce buttonYellowBouncer = Bounce(PIN_BUTTON_YELLOW, BUTTON_BOUNCE_DELAY);

// The LCD library object
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// The MP3 library object
MP3 mp3;

// The servo library object
Servo servo;

// The alarm variables
unsigned int alarm;
unsigned int alarmHours = 0;
unsigned int alarmInSeconds = 0;
unsigned int alarmMinutes = 0;
unsigned int alarmSeconds = 0;

// The button states
unsigned int buttonBlueState = LOW;
unsigned int buttonGreenState = LOW;
unsigned int buttonRedState = LOW;
unsigned int buttonYellowState = LOW;

// The number of sequences to display during the game
unsigned int numberOfSequences = 5;

// The time variables
unsigned int timeHours = 0;
unsigned int timeMinutes = 0;
unsigned int timeSeconds = 0;

/**
* Find out if the user has pressed any of the buttons
*/
void updateButtonStates() {
	// Update button bouncers
	buttonBlueChangedState = buttonBlueBouncer.update();
	buttonGreenChangedState = buttonGreenBouncer.update();
	buttonRedChangedState = buttonRedBouncer.update();
	buttonYellowChangedState = buttonYellowBouncer.update();

	// Read button bouncers
	buttonBlueState = buttonBlueBouncer.read();
	buttonGreenState = buttonGreenBouncer.read();
	buttonRedState = buttonRedBouncer.read();
	buttonYellowState = buttonYellowBouncer.read();
}

/**
* Blink single LED
*/
void blink(int pinToBlink, int timeToDelay) {
	// Create timer
	Timer timer;

	// Turn LED on
	digitalWrite(pinToBlink, HIGH);

	// Turn LED off after timeToDelay milliseconds
	while(true) {
		// Turn LED off
		if(timer.check(timeToDelay)) {
			digitalWrite(pinToBlink, LOW);
		}

		// Delay for effect and break loop
		if(timer.check(timeToDelay * 2)) {
			break;
		}
	}
}

/**
* Blink all LEDs
*/
void blinkAll(int timeToDelay) {
	// Create timer
	Timer timer;

	// Turn LEDs on
	digitalWrite(PIN_LED_BLUE, HIGH);
	digitalWrite(PIN_LED_GREEN, HIGH);
	digitalWrite(PIN_LED_RED, HIGH);
	digitalWrite(PIN_LED_YELLOW, HIGH);

	// Turn LEDs off after timeToDelay milliseconds
	while(true) {
		// Turn LEDs off
		if(timer.check(timeToDelay)) {
			digitalWrite(PIN_LED_BLUE, LOW);
			digitalWrite(PIN_LED_GREEN, LOW);
			digitalWrite(PIN_LED_RED, LOW);
			digitalWrite(PIN_LED_YELLOW, LOW);
		}

		// Delay for effect and break loop
		if(timer.check(timeToDelay * 2)) {
			break;
		}
	}
}

/**
* Print digits to LCD with leading zero
*/
void printDigitsToLCD(int digit) {
	// Print leading zero for 0–9
	if(digit < 10) {
		lcd.print("0");
	}
	
	// Print digit
	lcd.print(digit);
}

/**
* Print digits to console with leading zero
*/
void printDigitsToConsole(int digit) {
	// Print leading zero for 0–9
	if(digit < 10) {
		Serial.print("0");
	}
	
	// Print digit
	Serial.print(digit);
}

/**
* Prints the current time to LCD
*/
void printCurrentTimeToLCD() {
	// Print current time to console
	printCurrentTimeToConsole();

	// Print hour()
	lcd.clear();
	printDigitsToLCD(hour());

	// Print minute()
	lcd.print(":");
	printDigitsToLCD(minute());

	// Print second()
	lcd.print(":");
	printDigitsToLCD(second());
}

/**
* Prints the current time to console
*/
void printCurrentTimeToConsole() {
	// Print hour()
	printDigitsToConsole(hour());

	// Print minute()
	Serial.print(":");
	printDigitsToConsole(minute());

	// Print second()
	Serial.print(":");
	printDigitsToConsole(second());

	// Print newline
	Serial.println();
}

/**
* Prints any time to console
*/
void printTimeToConsole(int hour, int minute, int second) {
	// Print hour()
	printDigitsToConsole(hour);

	// Print minute
	Serial.print(":");
	printDigitsToConsole(minute);

	// Print second
	Serial.print(":");
	printDigitsToConsole(second);

	// Print newline
	Serial.println();
}

/**
* Print alarm status
*/
void printAlarmStatus() {
	// Set cursor to top right
	lcd.setCursor(15, 0);

	// Print asterisk if alarm is set and enabled, otherwise nothing
	if(isAlarmSet && isAlarmEnabled) {
		lcd.print("*");
	} else {
		lcd.print(" ");
	}
}

/**
* Listen for user actions during normal clock operation
*/
void listenForUserActions() {
	// Print alarm status
	printAlarmStatus();

	// Update button states
	updateButtonStates();

	// Set alarm
	if(!isAlarmEnabled && isTimeSet && buttonBlueState && buttonBlueChangedState) {
		setClockAlarm();
	}

	// Toggle alarm
	if(isAlarmSet && buttonYellowState && buttonYellowChangedState) {
		// Is the alarm enabled?
		if(isAlarmEnabled) {
			// Disable alarm
			Alarm.disable(alarm);

			// Set isAlarmEnabled to false
			isAlarmEnabled = false;

			// Print alarm status
			Serial.println("> Alarm has been disabled");
		} else {
			// Enable alarm
			Alarm.enable(alarm);

			// Set isAlarmEnabled to true
			isAlarmEnabled = true;

			// Print alarm status
			Serial.println("> Alarm has been enabled");
		}
	}
}

/**
* Initiate servo operation
*/
void initiateServo() {
	// Timer for delaying clock operation
	Timer timerClock;

	// Timer for delaying servo operation
	Timer timerServo;

	// The angle of the servo
	int servoAngle = 180;

	// The number of milliseconds the servo should use to turn 180 degrees
	float servoTime = (float) (alarmInSeconds * 1000);

	// The number of milliseconds the servo should use to turn 1 degree
	float servoDelay = servoTime / 180.0;

	// Print debug information
	Serial.print("> servoTime: ");
	Serial.println(servoTime);
	Serial.print("> servoDelay: ");
	Serial.println(servoDelay);

	// Print current time to LCD
	printCurrentTimeToLCD();

	// Begin servo operation
	while(true) {
		// Listen for user actions
		listenForUserActions();

		// Print current time
		if(timerClock.check(1000)) {
			// Print current time to LCD
			printCurrentTimeToLCD();

			// Reset timer
			timerClock.set();
		}

		// Turn servo
		if(timerServo.check(servoDelay)) {
			// Write angle to servo
			servo.write(servoAngle);

			// Decrement servoAngle
			servoAngle--;

			// Print debug information
			Serial.print("> servoAngle: ");
			Serial.println(servoAngle);

			// Reset timer
			timerServo.set();
		}

		// Break loop
		if(servoAngle == 0) {
			break;
		}
	}

	// Initiate sound operation
	initiateSound();
}

/**
* Initiate sound operation
*/
void initiateSound() {
	// Set MP3 shield to repeat mode
	mp3.set_mode(MP3::REPEAT);

	// Set volume to max
	mp3.volume(0x1F);

	// Generate new random seed
	randomSeed(analogRead(PIN_RANDOM));

	// Get random sound (from 1–5)
	int sound = random(1, 6);

	// Play sound
	switch(sound) {
		case 1:
			mp3.play_spi_flash(0x0001);
			break;
		case 2:
			mp3.play_spi_flash(0x0002);
			break;
		case 3:
			mp3.play_spi_flash(0x0003);
			break;
		case 4:
			mp3.play_spi_flash(0x0004);
			break;
		case 5:
			mp3.play_spi_flash(0x0005);
			break;
	}

	// Print debug information
	Serial.print("> Playing sound: ");
	Serial.println(sound);

	// Initiate game operation
	initiateGame();
}

/**
* Initiate game operation
*/
void initiateGame() {
	// Print alarm text to LCD
	lcd.clear();
	lcd.print("TIME TO");
	lcd.setCursor(0, 1);
	lcd.print("WAKE UP!");

	// Generate LED sequences
	for(int sequence = 0; sequence < numberOfSequences; sequence++) {
		// Whether or not the inputted sequence is correct
		boolean sequenceIsCorrect = true;

		// The number of buttons pressed
		int buttonsPressed = 0;
		
		// The sequence length
		int sequenceLength = sequence + 1;
		
		// The array that holds the sequence answer
		int sequenceArrayAnswer[sequenceLength];
		
		// The array that holds the user inputted sequence
		int sequenceArrayInput[sequenceLength];

		// Print debug information
		Serial.print("> Sequence: ");

		// Generate a LED sequence
		for(int segment = 0; segment < sequenceLength; segment++) {
			// Generate new random seed
			randomSeed(analogRead(PIN_RANDOM));

			// Get random LED pin (pins must be sequential from 15 to 19)
			sequenceArrayAnswer[segment] = random(15, 19);

			// Blink the corresponding LED
			blink(sequenceArrayAnswer[segment], 500);

			// Print debug information
			Serial.print(sequenceArrayAnswer[segment]);
			Serial.print(" ");
		}

		// Print newline to separate sequences
		Serial.println();

		// Listen for user input
		while(buttonsPressed < sequenceLength) {
			// Update button states
			updateButtonStates();

			// Add blue sequence segment
			if(buttonBlueState && buttonBlueChangedState) {
				// Add segment to input
				sequenceArrayInput[buttonsPressed] = PIN_LED_BLUE;
				
				// Count number of buttons pressed
				buttonsPressed++;

				// Print button status
				Serial.print("> Button: Blue (");
				Serial.print(buttonsPressed);
				Serial.println(");");
			}

			// Add red sequence segment
			if(buttonRedState && buttonRedChangedState) {
				// Add segment to input
				sequenceArrayInput[buttonsPressed] = PIN_LED_RED;
				
				// Count number of buttons pressed
				buttonsPressed++;

				// Print button status
				Serial.print("> Button: Red (");
				Serial.print(buttonsPressed);
				Serial.println(");");
			}

			// Add green sequence segment
			if(buttonGreenState && buttonGreenChangedState) {
				// Add segment to input
				sequenceArrayInput[buttonsPressed] = PIN_LED_GREEN;
				
				// Count number of buttons pressed
				buttonsPressed++;

				// Print button status
				Serial.print("> Button: Green (");
				Serial.print(buttonsPressed);
				Serial.println(");");
			}

			// Add yellow sequence segment
			if(buttonYellowState && buttonYellowChangedState) {
				// Add segment to input
				sequenceArrayInput[buttonsPressed] = PIN_LED_YELLOW;
				
				// Count number of buttons pressed
				buttonsPressed++;

				// Print button status
				Serial.print("> Button: Yellow (");
				Serial.print(buttonsPressed);
				Serial.println(");");
			}
		}

		// Test if inputted sequence is correct
		for(int i = 0; i < sequenceLength; i++) {
			if(sequenceArrayInput[i] != sequenceArrayAnswer[i]) {
				sequenceIsCorrect = false;
				break;
			}
		}

		// Was the sequence correct?
		if(sequenceIsCorrect) {
			// Blink green LED
			blink(PIN_LED_GREEN, 100);
			blink(PIN_LED_GREEN, 100);
			blink(PIN_LED_GREEN, 100);

			// Print sequence status				
			Serial.println("> Sequence is correct");
		} else {
			// Force user to start over
			sequence = -1;

			// Blink red LED
			blink(PIN_LED_RED, 100);
			blink(PIN_LED_RED, 100);
			blink(PIN_LED_RED, 100);

			// Print sequence status
			Serial.println("> Sequence is incorrect");
		}

		// Is this the last sequence?
		if((sequence + 1) != numberOfSequences) {
			// Print sequence separator
			Serial.println("----------");

			// Blink all LEDs to signal a new sequence
			blinkAll(1000);
		}
	}

	// Turn off sound
	mp3.stop();

	// Turn servo off
	servo.write(180);
}

/**
* Ask user to set the time
*/
void setClockTime() {
	// Create timer
	Timer timer;

	// Set timeHours
	if(!isTimeHoursSet) {
		// Clear display
		lcd.clear();

		// Print instructions
		lcd.print("PLEASE SET TIME");

		// Delay program
		timer.delay(1000);

		// Print instructions
		lcd.clear();
		lcd.print("--:--:--");
		lcd.setCursor(0, 1);
		lcd.print("Set hours");
		lcd.setCursor(0, 0);

		// Listen for user input
		while(!isTimeHoursSet) {
			// Update button states
			updateButtonStates();

			// Set timeHours
			if(buttonBlueState && buttonBlueChangedState) {
				isTimeHoursSet = true;
			}

			// Set timeHours up
			if(buttonGreenState && buttonGreenChangedState) {
				if(timeHours < 23) {
					timeHours++;
				}
			}

			// Set timeHours down
			if(buttonRedState && buttonRedChangedState) {
				if(timeHours > 0) {
					timeHours--;
				}
			}

			// Reset timeHours
			if(buttonYellowState && buttonYellowChangedState) {
				timeHours = 0;
			}

			// Set cursor
			lcd.setCursor(0, 0);

			// Print timeHours
			printDigitsToLCD(timeHours);
		}
	}

	// Set timeMinutes
	if(!isTimeMinutesSet) {
		// Print instructions
		lcd.setCursor(0, 1);
		lcd.print("Set minutes");
		lcd.setCursor(3, 0);

		// Listen for user input
		while(!isTimeMinutesSet) {
			// Update button states
			updateButtonStates();

			// Set timeMinutes
			if(buttonBlueState && buttonBlueChangedState) {
				isTimeMinutesSet = true;
			}

			// Set timeMinutes up
			if(buttonGreenState && buttonGreenChangedState) {
				if(timeMinutes < 59) {
					timeMinutes++;
				}
			}

			// Set timeMinutes down
			if(buttonRedState && buttonRedChangedState) {
				if(timeMinutes > 0) {
					timeMinutes--;
				}
			}

			// Reset timeMinutes
			if(buttonYellowState && buttonYellowChangedState) {
				timeMinutes = 0;
			}

			// Set cursor
			lcd.setCursor(3, 0);

			// Print timeMinutes
			printDigitsToLCD(timeMinutes);
		}
	}

	// Set timeSeconds
	if(!isTimeSecondsSet) {
		// Print instructions
		lcd.setCursor(0, 1);
		lcd.print("Set seconds");
		lcd.setCursor(3, 0);

		// Listen for user input
		while(!isTimeSecondsSet) {
			// Update button states
			updateButtonStates();

			// Set timeSeconds
			if(buttonBlueState && buttonBlueChangedState) {
				isTimeSecondsSet = true;
			}

			// Set timeSeconds up
			if(buttonGreenState && buttonGreenChangedState) {
				if(timeSeconds < 59) {
					timeSeconds++;
				}
			}

			// Set timeSeconds down
			if(buttonRedState && buttonRedChangedState) {
				if(timeSeconds > 0) {
					timeSeconds--;
				}
			}

			// Reset timeSeconds
			if(buttonYellowState && buttonYellowChangedState) {
				timeSeconds = 0;
			}

			// Set cursor
			lcd.setCursor(6, 0);

			// Print timeSeconds
			printDigitsToLCD(timeSeconds);
		}
	}

	// Set time
	setTime(timeHours, timeMinutes, timeSeconds, 1, 1, 2013);
	isTimeSet = true;

	// Print confirmation
	lcd.setCursor(0, 1);
	lcd.print("                ");
	lcd.setCursor(0, 1);
	lcd.print("Time is set!");

	// Delay program enough to see confirmation
	timer.delay(1000);
}

/**
* Ask user to set the alarm
*/
void setClockAlarm() {
	// Only allow alarm if time is set
	if(isTimeSet) {
		// Reset alarm booleans
		isAlarmSet = false;
		isAlarmHoursSet = false;
		isAlarmMinutesSet = false;

		// Create timer
		Timer timer;

		// Set timeHours
		if(!isAlarmHoursSet) {
			// Clear display
			lcd.clear();

			// Print instructions
			lcd.print("PLEASE SET ALARM");

			// Delay program
			timer.delay(1000);

			// Print instructions
			lcd.clear();
			lcd.print("--:--");
			lcd.setCursor(0, 1);
			lcd.print("Set hours");
			lcd.setCursor(0, 0);

			// Listen for user input
			while(!isAlarmHoursSet) {
				// Update button states
				updateButtonStates();

				// Set alarmHours
				if(buttonBlueState && buttonBlueChangedState) {
					isAlarmHoursSet = true;
				}

				// Set alarmHours up
				if(buttonGreenState && buttonGreenChangedState) {
					if(alarmHours < 23) {
						alarmHours++;
					}
				}

				// Set alarmHours down
				if(buttonRedState && buttonRedChangedState) {
					if(alarmHours > 0) {
						alarmHours--;
					}
				}

				// Reset alarmHours
				if(buttonYellowState && buttonYellowChangedState) {
					alarmHours = 0;
				}

				// Set cursor
				lcd.setCursor(0, 0);

				// Print alarmHours
				printDigitsToLCD(alarmHours);
			}
		}

		// Set alarmMinutes
		if(!isAlarmMinutesSet) {
			// Print instructions
			lcd.setCursor(0, 1);
			lcd.print("Set minutes");
			lcd.setCursor(3, 0);

			// Listen for user input
			while(!isAlarmMinutesSet) {
				// Update button states
				updateButtonStates();

				// Set alarmMinutes
				if(buttonBlueState && buttonBlueChangedState) {
					isAlarmMinutesSet = true;
				}

				// Set alarmMinutes up
				if(buttonGreenState && buttonGreenChangedState) {
					if(alarmMinutes < 59) {
						alarmMinutes++;
					}
				}

				// Set alarmMinutes down
				if(buttonRedState && buttonRedChangedState) {
					if(alarmMinutes > 0) {
						alarmMinutes--;
					}
				}

				// Reset alarmMinutes
				if(buttonYellowState && buttonYellowChangedState) {
					alarmMinutes = 0;
				}

				// Set cursor
				lcd.setCursor(3, 0);

				// Print alarmMinutes
				printDigitsToLCD(alarmMinutes);
			}
		}

		// Calculate alarmInSeconds
		alarmInSeconds = ((alarmHours - timeHours) * 60 * 60) + ((alarmMinutes - timeMinutes) * 60);

		// Set alarms
		if(alarmInSeconds >= 3600) {
			if(alarmMinutes == 0) {
				alarmHours--;
				alarmMinutes = 30;
			} else if(alarmMinutes > 30) {
				alarmHours--;
				alarmMinutes = -(alarmMinutes - 60);
			} else {
				alarmHours--;
				alarmMinutes = 60 + (alarmMinutes - 30);
			}
		} else {
			if(alarmMinutes % 2 == 0) {
				alarmMinutes = (alarmMinutes / 2);
			} else {
				alarmMinutes = (alarmMinutes / 2);
				alarmSeconds = 30;
			}
		}

		// Recalculate alarmInSeconds
		alarmInSeconds = ((alarmHours - timeHours) * 60 * 60) + ((alarmMinutes - timeMinutes) * 60) + (alarmSeconds - timeSeconds);

		// Set repeating alarm
		alarm = Alarm.alarmRepeat(alarmHours, alarmMinutes, alarmSeconds, initiateServo);

		// Alarm is set and enabled
		isAlarmSet = true;
		isAlarmEnabled = true;

		// Print confirmation
		lcd.setCursor(0, 1);
		lcd.print("                ");
		lcd.setCursor(0, 1);
		lcd.print("Alarm is set!");

		// Print alarm to console
		Serial.print("> Alarm has been set: ");
		printTimeToConsole(alarmHours, alarmMinutes, alarmSeconds);

		// Delay program enough to see confirmation
		timer.delay(1000);
	}
}

/**
* Setup program
*/
void setup() {
	// Initiate LCD object
	lcd.begin(16, 2);

	// Initiate MP3 object
	mp3.begin(MP3_HARDWARE_SERIAL);

	// Initiate servo object
	servo.attach(PIN_SERVO);
	servo.write(180);

	// Set pin modes
	pinMode(PIN_BUTTON_BLUE, INPUT);
	pinMode(PIN_BUTTON_GREEN, INPUT);
	pinMode(PIN_BUTTON_RED, INPUT);
	pinMode(PIN_BUTTON_YELLOW, INPUT);
	pinMode(PIN_LED_BLUE, OUTPUT);
	pinMode(PIN_LED_GREEN, OUTPUT);
	pinMode(PIN_LED_RED, OUTPUT);
	pinMode(PIN_LED_YELLOW, OUTPUT);

	// Begin serial output
	Serial.begin(9600);
	Serial.println();
}

/**
* Loop program
*/
void loop() {
	// Set time
	if(!isTimeSet) {
		setClockTime();
	}

	// Start clock
	if(isTimeSet) {
		// Create timer
		Timer timer;

		// Print current time
		printCurrentTimeToLCD();

		// Keep time
		while(true) {
			// Listen for user actions
			listenForUserActions();

			// Continue loop after one second
			if(timer.check(1000)) {
				// Delay alarm
				if(isAlarmSet) {
					Alarm.delay(0);
				}

				// Break loop
				break;
			}
		}
	}
}