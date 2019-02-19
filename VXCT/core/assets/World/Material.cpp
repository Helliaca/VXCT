#include "Material.h"



Material::Material(glm::vec3 color, float shininess, float ambient_str, float diffuse_str, float specular_str)
{
	setParams(color, shininess, ambient_str, diffuse_str, specular_str);
}


Material::~Material()
{
}


void Material::setParams(glm::vec3 color, float shininess, float ambient_str, float diffuse_str, float specular_str) {
	this->color = color;
	this->shininess = shininess;
	this->ambient_str = ambient_str;
	this->diffuse_str = diffuse_str;
	this->specular_str = specular_str;
}

Material_struct Material::to_struct() {
	Material_struct ret;
	ret.color = this->color;
	ret.shininess = this->shininess;
	ret.ambient_str = this->ambient_str;
	ret.diffuse_str = this->diffuse_str;
	ret.specular_str = this->specular_str;
	return ret;
}