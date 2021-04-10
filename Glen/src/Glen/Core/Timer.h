# pragma once 

#ifndef TIME_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Time {
public:
	Time();
	float getAsSeconds();
	float getAsMilliSeconds();
	Time& operator-(Time& other) {
		return Time(this->time - other.time);
	}
protected:
	Time(float seconds) { time = seconds; }
private:
	float time;
};

class Timer {
public:
	Time restart();
private:
	Time currentElapsedtime;
};
#endif // !TIME_H
