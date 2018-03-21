#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "base.h"
#include "globals.h"

enum PolygonMode { W_VERTICES, W_WIREFRAME, W_FILL };

class Window : IOobject
{
private:
	GLFWwindow * window;
public:
	void setPolygonMode(PolygonMode polygonmode = W_FILL, bool doubleSidedFaces = true);
	Window();
	~Window();
	bool shouldClose();
	GLFWwindow* getGLFWwindow();
};

#endif
