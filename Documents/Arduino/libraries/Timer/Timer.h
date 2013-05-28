#ifndef Timer_h
	#define Timer_h
	#include "Arduino.h"

	class Timer {
		public:
			Timer(void);
			void set(void);
			boolean check(int t);
			void delay(int t);
		private:
			unsigned long timer;
	};
#endif