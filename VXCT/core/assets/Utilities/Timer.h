#pragma once

#include "../../globals.h"

class Timer
{
public:
	Timer();
	~Timer();
	float elapsedTime();
	void start();
private:
	float startTime;
};

