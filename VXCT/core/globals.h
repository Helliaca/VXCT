#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "assets\camera.h"
#include "assets\PointLight.h"
#include "assets\Material.h"
#include "assets\VoxSettings.h"

namespace G {
	extern float deltaTime;
	extern float lastFrame;
	extern float lastX;
	extern float lastY;
	extern bool firstMouse;
	extern Camera* SceneCamera;
	extern PointLight* SceneLight;
	extern VoxSettings* VoxLightSettings;
}

#endif