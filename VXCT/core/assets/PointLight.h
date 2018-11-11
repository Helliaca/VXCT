#pragma once

#include "../base.h"

struct PointLight_struct {
	glm::vec3 position;
	glm::vec3 color;
	float att_constant;
	float att_linear;
	float att_quadratic;
};

class PointLight
{
public:
	PointLight(glm::vec3 position = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f), float att_constant = 1.0f, float att_linear = 0.15f, float att_quadratic = 0.45f);
	~PointLight();
	glm::vec3 position;
	glm::vec3 color;
	float att_constant;
	float att_linear;
	float att_quadratic;
	PointLight_struct to_struct();
	void setParams(glm::vec3 position, glm::vec3 color, float att_constant, float att_linear, float att_quadratic);
};

