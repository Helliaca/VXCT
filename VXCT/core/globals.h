#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "assets/Graphics/Camera.h"
#include "assets/World/Material.h"
#include "assets\VoxSettings.h"
#include "assets/World/Lighting.h"

namespace G {
	extern float deltaTime;
	extern float lastFrame;
	extern float lastX;
	extern float lastY;
	extern bool firstMouse;
	extern Camera* SceneCamera;
	extern Lighting* SceneLighting;
	extern VoxSettings* VoxLightSettings;
}

#endif