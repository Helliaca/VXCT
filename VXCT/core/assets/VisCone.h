#pragma once

#include "Model.h"

class VisCone : public Model
{
public:
	VisCone();
	~VisCone();
	void setConeParameters(glm::vec3 origin, glm::vec3 dir, float aperture_angle, float height);
private:
	float aperture_angle;
	float height;
};

