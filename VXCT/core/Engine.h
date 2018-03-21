#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "base.h"
#include "globals.h"
#include "Window.h"

class Engine : IOobject
{
private:
	void loadGlad();
public:
	Engine();
	~Engine();
	void run();
};

#endif