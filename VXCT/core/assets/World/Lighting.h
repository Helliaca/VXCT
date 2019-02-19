#pragma once

#include "PointLight.h"

class Lighting
{
public:
	std::vector<PointLight*> pointLights;
	void add_pointLight(PointLight* np);
	~Lighting();
	Lighting();
};