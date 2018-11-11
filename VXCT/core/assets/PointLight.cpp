#include "PointLight.h"



PointLight::PointLight(glm::vec3 position, glm::vec3 color, float att_constant, float att_linear, float att_quadratic)
{
	setParams(position, color, att_constant, att_linear, att_quadratic);
}


PointLight::~PointLight()
{
}

PointLight_struct PointLight::to_struct() {
	PointLight_struct ret;
	ret.att_constant = this->att_constant;
	ret.att_linear = this->att_linear;
	ret.att_quadratic = this->att_quadratic;
	ret.color = this->color;
	ret.position = this->position;
	return ret;
}

void PointLight::setParams(glm::vec3 position, glm::vec3 color, float att_constant, float att_linear, float att_quadratic) {
	this->position = position;
	this->color = color;
	this->att_constant = att_constant;
	this->att_linear = att_linear;
	this->att_quadratic = att_quadratic;
}