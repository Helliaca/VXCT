#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "camera.h"
#include "../LineRenderer.h"

namespace G {
	extern float deltaTime;
	extern float lastFrame;
	extern float lastX;
	extern float lastY;
	extern bool firstMouse;
	extern Camera* SceneCamera;
	extern LineRenderer* DebugLine;

	extern glm::vec3 lightPos;
	extern glm::vec3 lightColor;
}

#endif