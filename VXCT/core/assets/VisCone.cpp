#include "VisCone.h"



VisCone::VisCone() : Model("VisCone", RenderShader::EMITRGBA, OBJ_VIS_CONE)
{
	setConeParameters(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.9273f, 1.0f); //The default angle is calculated by 2*1*tan(0.5*x) = 1, as to achieve scales of 1, 1, 1 by default
}


VisCone::~VisCone()
{
}


void VisCone::setConeParameters(glm::vec3 origin, glm::vec3 dir, float aperture_angle, float height) {
	this->aperture_angle = aperture_angle;
	this->height = height;

	glm::vec3 new_scale = glm::vec3(2.0f*height*glm::tan(aperture_angle*0.5f), height, 2.0f*height*glm::tan(aperture_angle*0.5f));
	this->resetPSR();
	this->setPosition(origin); //Position NEEDS to be set before setting the angle
	this->lookAt(origin + dir);
	this->scale(new_scale);
}