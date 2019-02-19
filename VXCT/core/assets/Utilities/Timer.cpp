#include "Timer.h"


Timer::Timer()
{
	startTime = 0;
}


Timer::~Timer()
{
}


void Timer::start() {
	startTime = glfwGetTime();
}

float Timer::elapsedTime() {
	return glfwGetTime() - startTime;
}