#pragma once

#include "Model.h"

class SVCall
{
public:
	SVCall();
	~SVCall();
	Shader* callShader;
	void execute();
	unsigned int VBO, VAO;
};

