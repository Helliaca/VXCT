#version 450 core

struct Material {
	vec3 color;
	float ambient_str;
	float diffuse_str;
	float specular_str;
	float shininess;
};

out vec4 FragColor;

uniform Material material;

void main()
{
    FragColor = vec4(material.color, 1.0); // Simply emit the materials color flat
}