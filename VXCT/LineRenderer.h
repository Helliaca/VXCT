#pragma once

#ifndef LINERENDERER_H
#define LINERENDERER_H

#include "core\shader.h"

class LineRenderer : public IOobject
{
public:
	LineRenderer();
	~LineRenderer();
	void drawLine();

	std::vector<float> vertexData; //Includes vertices and Normals
	Shader* LineShader;
	unsigned int VBO, VAO;
	void draw();
};

#endif