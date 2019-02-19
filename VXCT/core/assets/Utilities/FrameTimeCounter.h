#pragma once

#include "Timer.h"

class FrameTimeCounter : IOobject
{
public:
	FrameTimeCounter(int frames);
	~FrameTimeCounter();
	void printLastFrames(int amount);
	void printAvg();
	void nextFrame();
	void start();
	void clear();
private:
	std::vector<float> frameTimes;
	int frames_to_save;
	float currentTime;
	Timer* innerclock;
};

