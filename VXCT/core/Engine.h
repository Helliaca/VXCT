#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "base.h"
#include "Window.h"
#include "../Scene.h"
#include "VoxelMap.h"
#include <mutex>
#include <thread>

class Engine : IOobject
{
private:
	void loadGlad();
	void console();
	std::thread consoleThread;
	std::mutex settingMutex;
public:
	VoxelMap * voxelMap;

	VoxelMap * voxelMap_lod1;

	void Voxelize(Scene* scene);
	Window* window;
	Engine();
	~Engine();
	void run();
	Scene* InitScene();
};

#endif