#include "Timer.h"

Time::Time()
{
	time = (float)glfwGetTime();
}

float Time::getAsSeconds()
{
	return time;
}

float Time::getAsMilliSeconds()
{
	return time * 1000;
}

Time Timer::restart()
{
	Time currentTime = Time();
	Time elapsedTime = currentTime - currentElapsedtime;
	currentElapsedtime = currentTime;
	return elapsedTime;
}