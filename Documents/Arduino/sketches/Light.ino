/**
* Include libraries
*/
#include <Bounce.h>

/**
* Set constants
*/
// Set button delay
const unsigned int BUTTON_BOUNCE_DELAY = 5;

// Set number of LED sequences to display
const unsigned int NUMBER_OF_SEQUENCES = 10;

// Set button pins
const unsigned int PIN_BUTTON_BLUE = 7;
const unsigned int PIN_BUTTON_GREEN = 8;
const unsigned int PIN_BUTTON_RED = 9;
const unsigned int PIN_BUTTON_YELLOW = 10;

// Set LED pins
const unsigned int PIN_LED_BLUE = 15;
const unsigned int PIN_LED_GREEN = 16;
const unsigned int PIN_LED_RED = 17;
const unsigned int PIN_LED_YELLOW = 18;

// Set random noise pin (must be analog and unused) 
const unsigned int PIN_RANDOM = 0;

/**
* Declare variables
*/
// Whether or not the button's state has changed
boolean buttonBlueChangedState = false;
boolean buttonGreenChangedState = false;
boolean buttonRedChangedState = false;
boolean buttonYellowChangedState = false;

// Whether or not the program is done
boolean done = false;

// The button bouncer objects (to disable multiple button presses at once)
Bounce buttonBlueBouncer = Bounce(PIN_BUTTON_BLUE, BUTTON_BOUNCE_DELAY);
Bounce buttonGreenBouncer = Bounce(PIN_BUTTON_GREEN, BUTTON_BOUNCE_DELAY);
Bounce buttonRedBouncer = Bounce(PIN_BUTTON_RED, BUTTON_BOUNCE_DELAY);
Bounce buttonYellowBouncer = Bounce(PIN_BUTTON_YELLOW, BUTTON_BOUNCE_DELAY);

// Whether or not the button is currently pressed
unsigned int buttonBlueState = LOW;
unsigned int buttonGreenState = LOW;
unsigned int buttonRedState = LOW;
unsigned int buttonYellowState = LOW;

/**
* Blink LED
*/
void blink(int pin, int timeToDelay) {
	// Record current time
	unsigned long timer = millis();

	// Turn LED on
	digitalWrite(pin, HIGH);

	// Delay program to blink LED
	while(true) {
		if((millis() - timer) >= timeToDelay) {
			// Turn LED off
			digitalWrite(pin, LOW);
		}

		if((millis() - timer) >= (timeToDelay * 2)) {
			break;
		}
	}
}

/**
* Blink all LED
*/
void blinkAll(int timeToDelay) {
	// Record current time
	unsigned long timer = millis();

	// Turn LED on
	digitalWrite(PIN_BUTTON_BLUE, HIGH);
	digitalWrite(PIN_BUTTON_GREEN, HIGH);
	digitalWrite(PIN_BUTTON_RED, HIGH);
	digitalWrite(PIN_BUTTON_YELLOW, HIGH);

	// Delay program to blink LED
	while(true) {
		if((millis() - timer) >= timeToDelay) {
			// Turn LED off
			digitalWrite(PIN_BUTTON_BLUE, LOW);
			digitalWrite(PIN_BUTTON_GREEN, LOW);
			digitalWrite(PIN_BUTTON_RED, LOW);
			digitalWrite(PIN_BUTTON_YELLOW, LOW);
		}

		if((millis() - timer) >= (timeToDelay * 2)) {
			break;
		}
	}
}

/**
* Setup program
*/
void setup() {
	// Set pin modes
	pinMode(PIN_BUTTON_BLUE, INPUT);
	pinMode(PIN_BUTTON_GREEN, INPUT);
	pinMode(PIN_BUTTON_RED, INPUT);
	pinMode(PIN_BUTTON_YELLOW, INPUT);
	pinMode(PIN_LED_BLUE, OUTPUT);
	pinMode(PIN_LED_GREEN, OUTPUT);
	pinMode(PIN_LED_RED, OUTPUT);
	pinMode(PIN_LED_YELLOW, OUTPUT);

	// Start serial output
	Serial.begin(9600);
}

/**
* Run program
*/
void loop() {
	// Run program
	if(!done) {
	

		// Set boolean to true
		done = true;
	}
}