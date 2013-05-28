#include "Arduino.h"
#include "Timer.h"

Timer::Timer(void) {
	set();
}

void Timer::set(void) {
	timer = millis();
}

boolean Timer::check(int t) {
	if((millis() - timer) >= t) {
		return true;
	}

	return false;
}

void Timer::delay(int t) {
	set();

	while(true) {
		if((millis() - timer) >= t) {
			break;
		}
	}
}