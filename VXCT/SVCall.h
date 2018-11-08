#pragma once

#include "Model.h"

//Class is not being used as of right now
class SVCall
{
public:
	SVCall();
	~SVCall();
	Shader* callShader;
	void execute();
	unsigned int VBO, VAO;
};

