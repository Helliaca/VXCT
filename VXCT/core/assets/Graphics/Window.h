#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include "../../globals.h"

enum PolygonMode { W_VERTICES, W_WIREFRAME, W_FILL };

class Window : IOobject
{
private:
	GLFWwindow * window;
	bool close;
public:
	void setPolygonMode(PolygonMode polygonmode = W_FILL, bool doubleSidedFaces = true);
	Window();
	~Window();
	bool shouldClose();
	void exit();
	GLFWwindow* getGLFWwindow();
	void processInput();
};

#endif
