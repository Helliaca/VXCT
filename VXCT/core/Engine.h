#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "base.h"
#include "Window.h"
#include "../Model.h"
#include "VoxelMap.h"

class Engine : IOobject
{
private:
	void loadGlad();
public:
	VoxelMap * voxelMap;
	void Voxelize();
	Window* window;
	Engine();
	~Engine();
	void run();
};

#endif