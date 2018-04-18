#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "camera.h"

namespace G {
	extern float deltaTime;
	extern float lastFrame;
	extern float lastX;
	extern float lastY;
	extern bool firstMouse;
	extern Camera* SceneCamera;

}

#endif