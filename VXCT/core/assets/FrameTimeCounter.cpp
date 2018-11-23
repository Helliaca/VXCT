#include "FrameTimeCounter.h"

#include <numeric>

FrameTimeCounter::FrameTimeCounter(int frames) : IOobject("FrameTimeCounter")
{
	frames_to_save = frames;
	innerclock = new Timer();
	innerclock->start();
}


FrameTimeCounter::~FrameTimeCounter()
{
}

void FrameTimeCounter::start() {
	innerclock->start();
}

void FrameTimeCounter::nextFrame() {
	frameTimes.push_back(innerclock->elapsedTime());
	innerclock->start();
	while (frameTimes.size() > frames_to_save) frameTimes.erase(frameTimes.begin());
}

void FrameTimeCounter::printLastFrames(int amount) {
	for (int i = 0; i<frameTimes.size() && i < amount; i++) {
		print(this, std::to_string(i) + ": " + std::to_string(frameTimes[i]) + "ms");
	}
	if (amount > frameTimes.size()) print(this, "WARN: Only " + std::to_string(frameTimes.size()) + " frames were collected, but " + std::to_string(amount) + " were requested.");
}

void FrameTimeCounter::printAvg() {
	float average = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0) / frameTimes.size();
	if (frameTimes.size() < frames_to_save) print(this, "WARN: Only " + std::to_string(frameTimes.size()) + " out of " + std::to_string(frames_to_save) + " frames were collected.");
	print(this, "Average frametime across last " + std::to_string(frameTimes.size()) + " frames: " + std::to_string(average) + "ms");
}

void FrameTimeCounter::clear() {
	frameTimes.clear();
	print(this, "All frametimes cleared.");
}