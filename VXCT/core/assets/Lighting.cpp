#include "Lighting.h"



Lighting::Lighting()
{
}


Lighting::~Lighting()
{
}

void Lighting::add_pointLight(PointLight* np) {
	this->pointLights.push_back(np);
}
