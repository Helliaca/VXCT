#pragma once

#include "../../base.h"

struct Material_struct {
	glm::vec3 color;
	float shininess;
	float ambient_str;
	float diffuse_str;
	float specular_str;
};

class Material
{
public:
	glm::vec3 color;
	float shininess;
	float ambient_str;
	float diffuse_str;
	float specular_str;
	Material_struct to_struct();
	Material(glm::vec3 color=glm::vec3(0.85f), float shininess=32.0f, float ambient_str=0.08f, float diffuse_str=1.0f, float specular_str=0.5f);
	~Material();
	void setParams(glm::vec3 color, float shininess, float ambient_str, float diffuse_str, float specular_str);
};

