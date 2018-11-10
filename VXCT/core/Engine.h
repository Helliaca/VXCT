#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "assets/Window.h"
#include "assets/Scene.h"
#include "assets/VoxelMap.h"
#include "assets/LineRenderer.h"
#include "assets/VisCone.h"
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

	Scene* mainScene;

	LineRenderer* DebugLine;

	VisCone* visCone;

	void Voxelize(Scene* scene);
	void visDetail(glm::vec3 fragPos, glm::vec3 fragNrm);
	Window* window;
	Engine();
	~Engine();
	void run();
	Scene* InitScene();
};

#endif