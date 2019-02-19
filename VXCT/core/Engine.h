#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "assets/Window.h"
#include "assets/VoxelMap.h"
#include "assets/LineRenderer.h"
#include "assets/FrameTimeCounter.h"
#include "assets/SceneParser.h"
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

	//VoxelMap * voxelMap_lod1;

	Scene* mainScene;

	LineRenderer* DebugLine;

	FrameTimeCounter* frametimecounter;

	Timer* revox_timer;

	Shader* voxelization_shader;

	void Voxelize(Scene* scene);
	Window* window;
	Engine();
	~Engine();
	void run();
	Scene* InitScene();
};

#endif