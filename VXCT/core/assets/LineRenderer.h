#pragma once

#include "shader.h"

class LineRenderer : IOobject
{
public:
	LineRenderer();
	~LineRenderer();

	std::vector<float> vertexData;
	Shader* shader;
	unsigned int VBO, VAO;
	void draw();
	void drawLine(glm::vec3 from, glm::vec3 to);
	void drawLine(glm::vec3 from, glm::vec3 dir, float length);

private:
	void updateMemory();
};

