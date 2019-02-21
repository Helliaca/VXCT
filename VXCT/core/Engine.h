#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "assets/Graphics/Window.h"
#include "assets/VoxelMap.h"
#include "assets/Utilities/LineRenderer.h"
#include "assets/Utilities/FrameTimeCounter.h"
#include "assets/Utilities/SceneParser.h"
#include <mutex>
#include <thread>

class Engine : IOobject
{
private:
	void loadGlad();
	void console();
	void toggle(bool* var, std::string name);
	std::thread consoleThread;
	std::mutex settingMutex;

public:
	VoxelMap * voxelMap;	// 3D texture

	Scene* mainScene;		// Scene

	LineRenderer* DebugLine;// LineRenderer

	FrameTimeCounter* frametimecounter;

	Timer* revox_timer;		// Timer for dynamic revoxelization

	Shader* voxelization_shader;

	Window* window;			// OpenGL context

	void run();
	void Voxelize(Scene* scene);

	Engine();
	~Engine();
};

#endif